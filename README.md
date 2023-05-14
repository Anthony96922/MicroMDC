# MicroMDC
Standalone MDC-1200 encoder (for GMRS and amateur radio)

This can create audio files with custom MDC-1200 data. Intended for GMRS but can be used for amateur radio too. Output files are 8 kHz sampling rate and 8 bit resolution. Licensed under GPLv3

This uses the MDC encoder library by Matthew Kaufman.

Requires libsndfile if wishing to use WAVE output otherwise C include output is only supported.
## Installing
````sh
git clone https://github.com/Anthony96922/MicroMDC
cd MicroMDC
git submodule update --init --recursive
make
````  
## Example usage
Short format:  
`./micromdc -o ani123.wav 01,00,0123`

Long format:  
`./micromdc -o doublepacket.wav 01,00,0123,45,67,89,ab`  
## License
GNU General Public License version 3  
