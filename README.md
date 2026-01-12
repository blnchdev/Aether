# Aether
![Example](https://www.blanche.dev/img/Aether/Aether1.png)

Ever wanted to call something like [NtQueueApcThreadEx2](https://ntdoc.m417z.com/ntqueueapcthreadex2) in your program without having to worry about version-dependent syscall IDX?

That's what Aether does. It lets you call undocumented Native API routines on any system without hardcoding IDX by Windows Build.  
This implementation assumes a non-modified ntdll.dll in your process.  
You can either implement your own skeletons and wrap the syscall stub inside of a function like so:

![Example Implementation](https://www.blanche.dev/img/Aether/Aether2.png)

Or simply use the Syscall function that allows you to dynamically call with the routine name:

![Example Implementation without Skeleton](https://www.blanche.dev/img/Aether/Aether3.png)

### Credits
[ComfySyscalls](https://www.unknowncheats.me/forum/c-and-c-/267587-comfy-direct-syscall-caller-x64.html) by namazso
