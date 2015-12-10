# efi-same
Utility for EFI systems that causes next boot to use same entry as one used for current system.
Useful for multi-boot systems or testing kernels on EFI-based systems.

Sets 'BootNext' variable to current value of 'BootCurrent', creating 'BootNext' if needed.

Requires efivar library and headers.

# Usage

Build the tool:

```$ make```

Run it:

```$ sudo ./efisame```

Expected result:

```
Created BootNext variable set to BootCurrent.
BootNext variable value verified.
```

# Using Systemd to Run efisame on Boot

Install the binary:

```$ sudo cp efisame /usr/local/bin```

Install and enable the provided systemd service:

```
$ sudo cp efisame.service /etc/systemd/system/
$ sudo systemctl enable efisame
```

Start the service to confirm it works:

```
$ sudo systemctl start efisame
$ sudo systemctl status efisame
```

Should give you something like:

```
● efisame.service - Set EFI to boot current entry next time
   Loaded: loaded (/etc/systemd/system/efisame.service; enabled; vendor preset: disabled)
   Active: active (exited) since Thu 2015-12-10 16:04:00 CST; 1s ago
  Process: 21655 ExecStart=/usr/local/bin/efisame (code=exited, status=0/SUCCESS)
 Main PID: 21655 (code=exited, status=0/SUCCESS)

Dec 10 16:04:00 thinkdtz systemd[1]: Starting Set EFI to boot current entry next time...
Dec 10 16:04:00 thinkdtz efisame[21655]: BootNext already set to BootCurrent.
Dec 10 16:04:00 thinkdtz efisame[21655]: BootNext variable value verified.
Dec 10 16:04:00 thinkdtz systemd[1]: Started Set EFI to boot current entry next time.
```

# Contact

(c) Will Dietz, 2015

efisame@wdtz.org

Enjoy!
