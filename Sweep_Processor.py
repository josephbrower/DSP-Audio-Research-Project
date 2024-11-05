import numpy as np
import scipy

import matplotlib.pyplot as plt


# Read in Files
samplerateRS, response = scipy.io.wavfile.read('SweepOutput.wav')
samplerateSweep, sweep = scipy.io.wavfile.read('Sweep.wav')

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

print(np.fft.fft2(sweep))
#IR = np.fft.fft2(response) / np.fft.fft2(sweep)
#print(IR)

#scipy.io.wavfile.write("TEST.wav", samplerateRS, response)
