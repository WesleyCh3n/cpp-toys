# MQTT vs HTTP

This experiment is to test out which protocol is faster to send an image.

## MQTT

Encode image to Base64 string and send it by MQTT. The subscriber then decode
the string back to image.

## HTTP

Using curl post file directly to http server. And save it to hard disk.
