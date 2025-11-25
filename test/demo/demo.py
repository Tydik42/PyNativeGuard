import sys
import os
import time
import zlib
import binascii

    
def try_custom_extension():
    current_dir = os.path.dirname(os.path.abspath(__file__))
    if current_dir not in sys.path:
        sys.path.append(current_dir)
        
    try:
        import test_hook
        print(f"[Python] Found custom C-extension. Triggering...")
        test_hook.trigger("TEST_MESSAGE")
        return True
    except ImportError as e:
        print(f"[Python] Custom 'test_hook' failed to import: {e}")
        return False

def try_standard_libs():
    print("[Python] Triggering via 'zlib.compress'...")
    data = b"x" * 1024
    zlib.compress(data, 1)

    print("[Python] Triggering via 'binascii.a2b_base64'...")
    try:
        binascii.a2b_base64(b"SGVsbG8=")
    except Exception:
        pass

def main():
    print(f"--- [Python Script Started] PID: {os.getpid()} ---")
    used_custom = try_custom_extension()
    
    if not used_custom:
        print("[Python] Falling back to standard libraries traffic.")
    
    for i in range(3):
        print(f"\n[Python] --- Iteration {i+1} ---")
        try_standard_libs()
        time.sleep(0.1)

    print("\n--- [Python Script Finished] ---")

if __name__ == "__main__":
    main()
