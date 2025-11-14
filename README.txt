Create a HaveIBeenPwned SQLite Database from a NTLM-hash text file
------------------------------------------------------------------

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
