// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

//extern int node_number;	//define the global variable

// TODO: reference additional headers your program requires here

#define CAN_ESD_NET 0
#define CAN_BAUDRATE 500000

#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <sstream>
#include <errno.h>
#include <windows.h>

#include "dsp402.h"
#include "input.h"
#include "canbase_esd.h"
#include "helper.h"
#include "nmt.h"
#include "sync.h"
#include "sdo.h"
#include "pdo.h"
#include "schunkpdo.h"

//content from MP's header file


//#include "targetver.h"

//#include <stdio.h>
//#include <tchar.h>

//#include <iostream>

#include "MPVector.h"
#include "MPMatrix.h"
#include "mslio.h"