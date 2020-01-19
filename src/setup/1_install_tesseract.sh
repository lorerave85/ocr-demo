#!/bin/bash

sudo add-apt-repository ppa:alex-p/tesseract-ocr
sudo apt-get update
sudo apt install -y tesseract-ocr
sudo apt install -y libtesseract-dev
