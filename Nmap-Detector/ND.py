#This code is written by chatgpt for those who don't want to use C program
#Feel free to modify

import os
import socket
import struct
import time
import subprocess

def trigger_tshark_capture(capture_count):
    """
    Trigger tshark to capture network traffic for 10 minutes and save it in captures_nmap directory.
    """
    filename = f"captures_nmap/pnmap{capture_count}.pcap"
    command = ["sudo", "tshark", "-i", "any", "-a", "duration:600", "-w", filename]

    # Run tshark as a subprocess
    subprocess.Popen(command)
    print(f"Started capturing traffic into: {filename}")

def detect_nmap_scan(packet):
    """
    Detects potential Nmap SYN scan by inspecting the TCP SYN flag.
    """
    ip_header = packet[0:20]
    iph = struct.unpack('!BBHHHBBH4s4s', ip_header)

    protocol = iph[6]
    if protocol == 6:  # Check if the protocol is TCP (6)
        tcp_header = packet[20:40]
        tcph = struct.unpack('!HHLLBBHHH', tcp_header)

        syn_flag = tcph[5] >> 1 & 1
        ack_flag = tcph[5] >> 4 & 1

        if syn_flag == 1 and ack_flag == 0:  # SYN set, ACK not set
            print("Potential Nmap SYN scan detected!")
            return True
    return False

def main():
    try:
        # Create a raw socket to listen for TCP packets
        sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
    except socket.error as msg:
        print(f"Socket could not be created. Error Code: {str(msg)}")
        return

    # Ensure the captures_nmap directory exists
    if not os.path.exists('captures_nmap'):
        os.makedirs('captures_nmap')

    capture_count = 1
    print("Listening for packets...")

    while True:
        # Receive packets
        packet = sock.recvfrom(65565)[0]

        # Detect Nmap scan
        if detect_nmap_scan(packet):
            trigger_tshark_capture(capture_count)
            capture_count += 1

            # Sleep for 10 minutes to avoid triggering multiple captures
            time.sleep(600)

if __name__ == "__main__":
    main()

