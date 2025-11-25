import ctypes
import sys

def trigger_via_ctypes():
    print("[Python] Triggering via ctypes (Direct C-API call)...")
    
    lib = ctypes.CDLL(None)
    parse_tuple = lib.PyArg_ParseTuple    
    try:
        pass 
    except:
        pass

    pyapi = ctypes.pythonapi
    
    dummy_args = (1, 2)
    py_tuple = ctypes.py_object(dummy_args)
    
    fmt = b"ii"
    
    a = ctypes.c_int()
    b = ctypes.c_int()
    
    pyapi.PyArg_ParseTuple(py_tuple, fmt, ctypes.byref(a), ctypes.byref(b))
    
    print(f"[Python] Ctypes call finished. Parsed: {a.value}, {b.value}")

if __name__ == "__main__":
    trigger_via_ctypes()
