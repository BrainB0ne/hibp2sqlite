Create a HaveIBeenPwned SQLite Database from a NTLM-hash text file
------------------------------------------------------------------

Build Requirements:

[Windows]
- Microsoft Visual Studio 2022
- Qt SDK 6.10.x / Qt Creator 18

To obtain the NTLM-hash text file, use PwnedPasswordsDownloader from HaveIBeenPwned.

https://github.com/HaveIBeenPwned/PwnedPasswordsDownloader

Install PwnedPasswordsDownloader by following the instructions described on the link above.

Then, download all NTLM hashes to a single txt file called pwnedpasswords_ntlm.txt
haveibeenpwned-downloader.exe -n pwnedpasswords_ntlm

Caution!
Make sure there is enough free space on your drive since this file is at the time of writing ~70 GB.
The conversion to SQLite with hibp2sqlite takes another ~180 GB for the SQLite database file.

Usage: hibp2sqlite.exe [options] source destination

Options:
  -?, -h, --help  Displays help on commandline options.
  -v, --version   Displays version information.

Arguments:
  source          Source NTLM-hash text file to process.
  destination     Destination SQLite database.

Note: the examples folder contains a small text file with 3 NTLM hashes for testing purposes.

If you are looking for a GUI application to start this console application and query the SQLite database, check out: haveibeenpwned-local
More Information: https://github.com/BrainB0ne/haveibeenpwned-local or https://codeberg.org/BrainB0ne/haveibeenpwned-local

Credits to the author of the following article since it gave me the idea to create this tool.
https://medium.com/analytics-vidhya/creating-a-local-version-of-the-haveibeenpwned-password-database-with-python-and-sqlite-918a7b6a238a

And last, but not least... Read the disclaimer below. :)

Disclaimer
* I'm not responsible for bricked devices, dead SD cards, thermonuclear war, or you getting fired because the alarm app failed.
* YOU are choosing to run these tools, and if you point the finger at me for messing up your device, I will laugh at you.
