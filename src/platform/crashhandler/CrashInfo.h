/*
 * Copyright 2011 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ARX_PLATFORM_CRASHHANDLER_CRASHINFO_H
#define ARX_PLATFORM_CRASHHANDLER_CRASHINFO_H

#include "Configure.h"

#define BOOST_DATE_TIME_NO_LIB
#include <boost/version.hpp>
#if BOOST_VERSION < 104500
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-fpermissive"
#endif
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#if BOOST_VERSION < 104500
#pragma GCC diagnostic pop
#endif

#include "platform/Platform.h"
#include "platform/Thread.h"

#ifdef HAVE_GETRUSAGE
#include "sys/resource.h"
#endif

struct CrashInfoBase {
	
	CrashInfoBase() : exitLock(0) { }
	
	enum Constants {
		MaxNbFiles = 32,
		MaxFilenameLen = 256,
		MaxNbVariables = 64,
		MaxVariableNameLen = 64,
		MaxVariableValueLen = 128,
		MaxDetailCrashInfoLen = 4096,
		MaxCallstackDepth = 256
	};
	
	// Files to attach to the report.
	int	 nbFilesAttached;
	char attachedFiles[MaxNbFiles][MaxFilenameLen];
	
	// Variables to add to the report.
	int nbVariables;
	struct Variable {
		char name[MaxVariableNameLen];
		char value[MaxVariableValueLen];
	} variables[MaxNbVariables];
	
	// ID of the crashed process & thread
	process_id_type processId;
	thread_id_type threadId;
	
	// Detailed crash info (messages, registers, whatever).
	char detailedCrashInfo[MaxDetailCrashInfoLen];
	
	// Where the crash reports should be written.
	char crashReportFolder[MaxFilenameLen];
	
	// Once released, this lock will allow the crashed application to terminate.
	boost::interprocess::interprocess_semaphore	exitLock;
	
};


#if ARX_PLATFORM != ARX_PLATFORM_WIN32

struct CrashInfo : public CrashInfoBase {
	char execFullName[512];
	char backtrace[4096];
	bool have_rusage;
#ifdef HAVE_GETRUSAGE
	struct ::rusage rusage;
#endif
};

#else

#include <windows.h>
#include <dbghelp.h>

struct CrashInfo : public CrashInfoBase {
	EXCEPTION_RECORD exceptionRecord;
	CONTEXT          contextRecord;
	MINIDUMP_TYPE    miniDumpType;
	HANDLE           threadHandle;
};

#endif

#endif // ARX_PLATFORM_CRASHHANDLER_CRASHINFO_H
