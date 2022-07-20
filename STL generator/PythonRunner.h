#pragma once
#include <Python.h>


void test()
{
	Py_Initialize();
	PyRun_SimpleString("from time import time,ctime\n"
		"from rich import print\n"
		"print('Today is',ctime(time()))\n");
}