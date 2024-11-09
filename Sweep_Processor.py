###################################################
# Authors: Ben Polzin, Joseph Brower
# Inputs a Dry Sine Sweep and a Recorded Sine Sweep
# and Outputs the Impulse Response
###################################################

import numpy as np
import scipy

# Read in Files
samplerateRS, response = scipy.io.wavfile.read('Test Files/Sweeps/SweepSophia.wav')
samplerateSweep, sweep = scipy.io.wavfile.read('Test Files/Sweeps/Sweep.wav')

# Normalize Audio
response = response / np.max(np.abs(response))
sweep = sweep / np.max(np.abs(sweep))

# Convert Mono to Stereo
if response.ndim == 1:
    response = np.vstack([response, response])
    response = np.transpose(response)

if sweep.ndim == 1:
    sweep = np.vstack([sweep, sweep])
    sweep = np.transpose(sweep)

# Match Lengths of Arrays by Padding with Zeros
if len(response) > len(sweep):
    sweep_pad = np.zeros((len(response), 2))
    sweep_pad[:len(sweep)] = sweep
    sweep = sweep_pad

if len(sweep) > len(response):
    response_pad = np.zeros((len(sweep), 2))
    response_pad[:len(response)] = response
    response = response_pad

# DeConvolve Audio
response_left, response_right = np.split(response, 2, axis=1)
sweep_left, sweep_right = np.split(sweep, 2, axis=1)

response_left = np.transpose(response_left)[0]
response_right = np.transpose(response_right)[0]
sweep_left = np.transpose(sweep_left)[0]
sweep_right = np.transpose(sweep_right)[0]

IR_left = scipy.fft.ifft( scipy.fft.fft(response_left) / scipy.fft.fft(sweep_left) ).real
IR_left = IR_left / np.max(np.abs(IR_left))

IR_right = scipy.fft.ifft( scipy.fft.fft(response_right) / scipy.fft.fft(sweep_right) ).real
IR_right = IR_right / np.max(np.abs(IR_right))

IR = np.vstack([IR_left, IR_right])
IR = np.transpose(IR)

# Output IR to a Wav File
scipy.io.wavfile.write("RecoveredImpulseResponse.wav", samplerateRS, IR)