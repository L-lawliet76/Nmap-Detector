
# Nmap Detector with Tshark Capture

This project consists of two versions of a Nmap detector program (in C and Python). The programs listen for potential Nmap SYN scans on the network and, upon detection, automatically trigger `tshark` to capture network traffic for 10 minutes. The capture files are saved in a `captures_nmap` directory.You can modify the program to block the ip too.

## Requirements

- **C Version**: Requires a C compiler (`gcc`), `tshark`, and root privileges.
- **Python Version**: Requires Python 3.x, `tshark`, and root privileges.
- Root privileges to run the program and capture network traffic.

## Installation

### For the C Version:

1. Install `tshark` (part of Wireshark):
   ```bash
   sudo apt-get install tshark
   ```

2. Compile the C program:
   ```bash
   gcc -o ND ND.c
   ```

3. Make sure you have the `captures_nmap` directory:
   ```bash
   mkdir captures_nmap
   ```

4. Set the correct permissions:
   ```bash
   chmod 777 ~/your/path/to/Nmap-Detector
   chmod 777 captures_nmap
   ```

### For the Python Version:

1. Install Python 3.x if you don't have it:
   ```bash
   sudo apt-get install python3
   ```

2. Install `tshark`:
   ```bash
   sudo apt-get install tshark
   ```

3. Make sure you have the `captures_nmap` directory:
   ```bash
   mkdir captures_nmap
   ```

4. Set the correct permissions:
   ```bash
   chmod 777 ~/your/path/to/Nmap-Detector
   chmod 777 captures_nmap
   ```

## Running the Program

### For the C Version:

1. Run the C program with `sudo` to give it the necessary permissions to capture network traffic:
   ```bash
   sudo ./ND
   ```

2. The program will:
   - Listen for incoming TCP packets.
   - Detect potential Nmap SYN scans.
   - Trigger a `tshark` capture for 10 minutes when a scan is detected.
   - Save the capture files in the `captures_nmap` directory as `cnmap1.pcap`, `pnmap2.pcap`, etc.

### For the Python Version:

1. Run the Python program with `sudo`:
   ```bash
   sudo python3 ND.py
   ```

2. The program will:
   - Listen for incoming TCP packets.
   - Detect potential Nmap SYN scans.
   - Trigger a `tshark` capture for 10 minutes when a scan is detected.
   - Save the capture files in the `captures_nmap` directory as `NMap1.pcap`, `NMap2.pcap`, etc.

## License
This project is licensed under the MIT License.
