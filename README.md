# Comfier Syscalls
![Example Implementation](https://www.blanche.dev/img/ComfierSyscalls.png)
Ever wanted to call something like [NtQueueApcThreadEx2](https://ntdoc.m417z.com/ntqueueapcthreadex2) in your program without having to worry about version-dependent syscalls IDX?  
That's what Comfier Syscalls achieves for you; it allows you to call undocumented Native API routines on any system without having to hardcode IDX by Windows Build  
This implementation assumes a non-modified ntdll.dll in your process!  

### Credits
[ComfySyscalls](https://www.unknowncheats.me/forum/c-and-c-/267587-comfy-direct-syscall-caller-x64.html) by namazso
