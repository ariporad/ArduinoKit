//
//  Communicator.swift
//  ArduinoKit
//
//  Created by School on 9/7/14.
//  Copyright (c) 2014 Ari Porad. All rights reserved.
//

import Foundation
import IOKit
import sys

// This will take care of connecting to the arduino. It will connect to the first one running ArduinoKit it finds.
func connect() {
	var ioObject: io_object_t;
	var ioIterator: io_iterator_t;
	
	IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching(kIOSerialBSDServiceValue), &ioIterator);
	
}