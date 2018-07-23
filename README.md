# Attenuator
Arduino code for a variable-attenuator control loop.

This software controls two variable attenuators on two independent channels. Each channel supports a "Manual" or an "Automatic" mode. Manual attenuation is controlled using a potentiometer attached to the Arduino. In the automatic attenuation mode, the Arduino references an analog signal (0-5V) as the desired attenuation. This analog value is created by an external desktop computer running a control loop to determine the desired attenuation.
