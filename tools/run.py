import os
import sys
import subprocess
import argparse
from pathlib import Path

PROJECT_ROOT = Path(__file__).parent.parent.resolve()

SEARCH_PATHS = [
    PROJECT_ROOT / "build" / "lib",
    PROJECT_ROOT / "build" / "src" / "agent",
    PROJECT_ROOT / "out" / "build" / "linux-debug" / "lib",
]

LIB_NAME = "libpynativeguard_agent.so"

def find_agent_library():
    for path in SEARCH_PATHS:
        candidate = path / LIB_NAME
        if candidate.exists():
            return candidate
            
    build_root = PROJECT_ROOT / "build"
    if build_root.exists():
        found = list(build_root.rglob(LIB_NAME))
        if found:
            return found[0]
            
    return None

def main():
    parser = argparse.ArgumentParser(description="Run Python script with PyNativeGuard agent")
    parser.add_argument("script", nargs=argparse.REMAINDER, help="Target script and its arguments")
    args = parser.parse_args()

    if not args.script:
        print("Usage: ./tools/run.py [script.py] [args...]")
        sys.exit(1)

    agent_path = find_agent_library()
    if not agent_path:
        print(f"Error: Could not find '{LIB_NAME}'.")
        print("Did you build the project? Try: cmake --build build")
        print(f"Searched in: {[str(p) for p in SEARCH_PATHS]}")
        sys.exit(1)

    env = os.environ.copy()
    current_preload = env.get("LD_PRELOAD", "")
    env["LD_PRELOAD"] = f"{agent_path}:{current_preload}" if current_preload else str(agent_path)
    
    env["PYTHONUNBUFFERED"] = "1"

    print(f"[*] Injecting: {agent_path}")
    print(f"[*] Executing: {' '.join(args.script)}")
    print("-" * 50)

    try:
        subprocess.run([sys.executable] + args.script, env=env, check=True)
    except subprocess.CalledProcessError as e:
        sys.exit(e.returncode)
    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    main()
