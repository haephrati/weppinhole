///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2010 - <company name here>
///
/// Original filename: WifiCapture.h
/// Project          : WifiCapture
/// Date of creation : <see WifiCapture.cpp>
/// Author(s)        : <see WifiCapture.cpp>
///
/// Purpose          : <see WifiCapture.cpp>
///
/// Revisions:         <see WifiCapture.cpp>
///
///////////////////////////////////////////////////////////////////////////////

// $Id$

#ifndef __WIFICAPTURE_H_VERSION__
#define __WIFICAPTURE_H_VERSION__ 100

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#include "drvcommon.h"
#include "drvversion.h"

#define DEVICE_NAME			"\\Device\\WIFICAPTURE_DeviceName"
#define SYMLINK_NAME		"\\DosDevices\\WIFICAPTURE_DeviceName"
PRESET_UNICODE_STRING(usDeviceName, DEVICE_NAME);
PRESET_UNICODE_STRING(usSymlinkName, SYMLINK_NAME);

#ifndef FILE_DEVICE_WIFICAPTURE
#define FILE_DEVICE_WIFICAPTURE 0x8000
#endif

// Values defined for "Method"
// METHOD_BUFFERED
// METHOD_IN_DIRECT
// METHOD_OUT_DIRECT
// METHOD_NEITHER
// 
// Values defined for "Access"
// FILE_ANY_ACCESS
// FILE_READ_ACCESS
// FILE_WRITE_ACCESS

const ULONG IOCTL_WIFICAPTURE_OPERATION = CTL_CODE(FILE_DEVICE_WIFICAPTURE, 0x01, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA);

#endif // __WIFICAPTURE_H_VERSION__
