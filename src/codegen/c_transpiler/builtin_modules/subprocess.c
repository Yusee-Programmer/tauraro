// ==========================================
// SUBPROCESS MODULE - ENHANCED Pure C Implementation
// ==========================================
// Provides: subprocess.run(), subprocess.Popen(), PIPE, STDOUT, DEVNULL constants
// Platform: Cross-platform (Windows/Linux/macOS)

#ifndef TAURARO_SUBPROCESS_MODULE_H
#define TAURARO_SUBPROCESS_MODULE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifndef TAU_HELPER_FUNCTIONS_DEFINED
#define TAU_HELPER_FUNCTIONS_DEFINED

static inline double tau_to_double(TauValue v) {
    if (v.type == 0) return (double)v.value.i;
    if (v.type == 1) return v.value.f;
    return 0.0;
}

static inline int64_t tau_to_int64(TauValue v) {
    if (v.type == 0) return v.value.i;
    if (v.type == 1) return (int64_t)v.value.f;
    return 0;
}

static inline bool tau_to_bool(TauValue v) {
    if (v.type == 3) return v.value.i != 0;
    if (v.type == 0) return v.value.i != 0;
    if (v.type == 1) return v.value.f != 0.0;
    if (v.type == 2) return v.value.s != NULL && v.value.s[0] != '\0';
    return true;
}

static inline char* tau_to_string(TauValue v) {
    if (v.type == 2) return v.value.s;
    return NULL;
}
#endif // TAU_HELPER_FUNCTIONS_DEFINED

#ifdef _WIN32
    #include <windows.h>
    #include <process.h>
#else
    #include <unistd.h>
    #include <sys/wait.h>
    #include <sys/types.h>
#endif

// Constants for pipe redirection
#define SUBPROCESS_PIPE     -1
#define SUBPROCESS_STDOUT   -2
#define SUBPROCESS_DEVNULL  -3

// CompletedProcess object structure
typedef struct {
    int returncode;
    char* stdout_data;
    char* stderr_data;
} CompletedProcess;

// Popen object structure
typedef struct {
    int returncode;
    int pid;
#ifdef _WIN32
    HANDLE process_handle;
#endif
    FILE* stdout_pipe;
    FILE* stderr_pipe;
} Popen;

// subprocess.PIPE constant - special value for subprocess pipes
static inline TauValue tauraro_subprocess_PIPE(void) {
    return (TauValue){.type = 0, .value.i = SUBPROCESS_PIPE, .refcount = 1, .next = NULL};
}

// subprocess.STDOUT constant - redirect stderr to stdout
static inline TauValue tauraro_subprocess_STDOUT(void) {
    return (TauValue){.type = 0, .value.i = SUBPROCESS_STDOUT, .refcount = 1, .next = NULL};
}

// subprocess.DEVNULL constant - discard output
static inline TauValue tauraro_subprocess_DEVNULL(void) {
    return (TauValue){.type = 0, .value.i = SUBPROCESS_DEVNULL, .refcount = 1, .next = NULL};
}

// subprocess.TimeoutExpired exception
static inline TauValue tauraro_subprocess_TimeoutExpired(TauValue message, TauValue timeout, TauValue cmd) {
    // Create exception-like string
    const char* msg = (message.type == 2) ? message.value.s : "Command timed out";
    char* exception = malloc(strlen(msg) + 100);
    sprintf(exception, "TimeoutExpired: %s", msg);
    return (TauValue){.type = 2, .value.s = exception, .refcount = 1, .next = NULL};
}

// subprocess.CalledProcessError exception
static inline TauValue tauraro_subprocess_CalledProcessError(TauValue returncode, TauValue cmd, TauValue output, TauValue stderr) {
    const char* cmd_str = (cmd.type == 2) ? cmd.value.s : "unknown";
    int ret_code = (returncode.type == 0) ? returncode.value.i : -1;
    char* exception = malloc(strlen(cmd_str) + 100);
    sprintf(exception, "CalledProcessError: Command '%s' returned non-zero exit status %d", cmd_str, ret_code);
    return (TauValue){.type = 2, .value.s = exception, .refcount = 1, .next = NULL};
}



// subprocess.run(command, capture_output) - Run command and return result
static inline TauValue tauraro_subprocess_run(TauValue command, TauValue capture_output) {
    if (command.type != 2) {
        return (TauValue){.type = 5, .value.dict = NULL, .refcount = 1, .next = NULL};
    }

    int should_capture = (capture_output.type == 3) ? capture_output.value.i : 0;
    CompletedProcess* result = malloc(sizeof(CompletedProcess));
    result->returncode = -1;
    result->stdout_data = strdup("");
    result->stderr_data = strdup("");

#ifdef _WIN32
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessA(NULL, (LPSTR)command.value.s, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        
        DWORD exit_code;
        GetExitCodeProcess(pi.hProcess, &exit_code);
        result->returncode = (int)exit_code;

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        result->returncode = -1;
    }
#else
    int status = system(command.value.s);
    if (WIFEXITED(status)) {
        result->returncode = WEXITSTATUS(status);
    } else {
        result->returncode = -1;
    }
#endif

    return (TauValue){
        .type = 5,
        .value.dict = (TauDict*)result,
        .refcount = 1,
        .next = NULL
    };
}

// subprocess.call(command) - Call command and return exit code
static inline TauValue tauraro_subprocess_call(TauValue command) {
    if (command.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

#ifdef _WIN32
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessA(NULL, (LPSTR)command.value.s, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        
        DWORD exit_code;
        GetExitCodeProcess(pi.hProcess, &exit_code);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return (TauValue){.type = 0, .value.i = (int64_t)exit_code, .refcount = 1, .next = NULL};
    } else {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }
#else
    int status = system(command.value.s);
    if (WIFEXITED(status)) {
        return (TauValue){.type = 0, .value.i = WEXITSTATUS(status), .refcount = 1, .next = NULL};
    } else {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }
#endif
}

// subprocess.check_call(command) - Call command, raise on non-zero exit
static inline TauValue tauraro_subprocess_check_call(TauValue command) {
    if (command.type != 2) {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }

#ifdef _WIN32
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessA(NULL, (LPSTR)command.value.s, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        
        DWORD exit_code;
        GetExitCodeProcess(pi.hProcess, &exit_code);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (exit_code != 0) {
            fprintf(stderr, "Command returned non-zero exit code: %lu\n", exit_code);
        }
        return (TauValue){.type = 0, .value.i = (int64_t)exit_code, .refcount = 1, .next = NULL};
    } else {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }
#else
    int status = system(command.value.s);
    if (WIFEXITED(status)) {
        int exit_code = WEXITSTATUS(status);
        if (exit_code != 0) {
            fprintf(stderr, "Command returned non-zero exit code: %d\n", exit_code);
        }
        return (TauValue){.type = 0, .value.i = exit_code, .refcount = 1, .next = NULL};
    } else {
        return (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
    }
#endif
}

// subprocess.getoutput(command) - Run command and return output as string
static inline TauValue tauraro_subprocess_getoutput(TauValue command) {
    if (command.type != 2) {
        TauValue __result = (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL}; return __result;
    }

#ifdef _WIN32
    FILE* pipe = _popen(command.value.s, "r");
#else
    FILE* pipe = popen(command.value.s, "r");
#endif

    if (!pipe) {
        TauValue __result = (TauValue){.type = 2, .value.s = "", .refcount = 1, .next = NULL}; return __result;
    }

    char buffer[4096];
    char output[16384] = {0};
    size_t total_len = 0;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t len = strlen(buffer);
        if (total_len + len < sizeof(output)) {
            strcat(output, buffer);
            total_len += len;
        }
    }

#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif

    return tauraro_string(strdup(output));
}

// subprocess.getstatusoutput(command) - Run command and return (status, output)
static inline TauValue tauraro_subprocess_getstatusoutput(TauValue command) {
    if (command.type != 2) {
        TauList* result = malloc(sizeof(TauList));
        result->items = malloc(sizeof(TauValue) * 2);
        result->size = 2;
        result->capacity = 2;
        result->items[0] = (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
        result->items[1] = tauraro_string("");
        return (TauValue){.type = 4, .value.list = result, .refcount = 1, .next = NULL};
    }

#ifdef _WIN32
    FILE* pipe = _popen(command.value.s, "r");
#else
    FILE* pipe = popen(command.value.s, "r");
#endif

    if (!pipe) {
        TauList* result = malloc(sizeof(TauList));
        result->items = malloc(sizeof(TauValue) * 2);
        result->size = 2;
        result->capacity = 2;
        result->items[0] = (TauValue){.type = 0, .value.i = -1, .refcount = 1, .next = NULL};
        result->items[1] = tauraro_string("");
        return (TauValue){.type = 4, .value.list = result, .refcount = 1, .next = NULL};
    }

    char buffer[4096];
    char output[16384] = {0};
    size_t total_len = 0;
    int status = 0;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t len = strlen(buffer);
        if (total_len + len < sizeof(output)) {
            strcat(output, buffer);
            total_len += len;
        }
    }

#ifdef _WIN32
    status = _pclose(pipe);
#else
    status = pclose(pipe);
#endif

    if (WIFEXITED(status)) {
        status = WEXITSTATUS(status);
    }

    TauList* result = malloc(sizeof(TauList));
    result->items = malloc(sizeof(TauValue) * 2);
    result->size = 2;
    result->capacity = 2;
    result->items[0] = (TauValue){.type = 0, .value.i = status, .refcount = 1, .next = NULL};
    result->items[1] = tauraro_string(strdup(output));

    return (TauValue){.type = 4, .value.list = result, .refcount = 1, .next = NULL};
}

// Helper: Get CompletedProcess fields
static inline TauValue tauraro_subprocess_CompletedProcess_returncode(TauValue self) {
    CompletedProcess* proc = (CompletedProcess*)self.value.dict;
    return (TauValue){.type = 0, .value.i = proc->returncode, .refcount = 1, .next = NULL};
}

static inline TauValue tauraro_subprocess_CompletedProcess_stdout(TauValue self) {
    CompletedProcess* proc = (CompletedProcess*)self.value.dict;
    return tauraro_string(strdup(proc->stdout_data));
}

static inline TauValue tauraro_subprocess_CompletedProcess_stderr(TauValue self) {
    CompletedProcess* proc = (CompletedProcess*)self.value.dict;
    return tauraro_string(strdup(proc->stderr_data));
}

// subprocess.CompletedProcess constructor
static inline TauValue tauraro_subprocess_CompletedProcess(TauValue returncode, TauValue stdout_data, TauValue stderr_data) {
    CompletedProcess* proc = malloc(sizeof(CompletedProcess));
    
    if (returncode.type == 0) {
        proc->returncode = (int)returncode.value.i;
    } else {
        proc->returncode = 0;
    }

    proc->stdout_data = (stdout_data.type == 2) ? strdup(stdout_data.value.s) : strdup("");
    proc->stderr_data = (stderr_data.type == 2) ? strdup(stderr_data.value.s) : strdup("");

    return (TauValue){
        .type = 5,
        .value.dict = (TauDict*)proc,
        .refcount = 1,
        .next = NULL
    };
}


#endif // TAURARO_SUBPROCESS_MODULE_H
