#!/usr/bin/python

import signal
import time

# Signal handler for SIGTERM (does nothing)
def handle_sigterm(sig, frame):
    pass

# Set the signal handler for SIGTERM to the handle_sigterm function
signal.signal(signal.SIGTERM, handle_sigterm)

# Print a message every second
print("Running... Press Ctrl+C to exit.")
while True:
    print("Working...")
    time.sleep(10)
