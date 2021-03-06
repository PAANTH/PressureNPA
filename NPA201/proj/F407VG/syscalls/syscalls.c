/* Includes */
#include <stdint.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
//#include <sys/time.h>
#include <sys/times.h>


/* Variables */
#undef errno
extern int32_t errno;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

register uint8_t * stack_ptr __asm("sp");

uint8_t *__env[1] = { 0 };
uint8_t **environ = __env;

void __attribute__ ((weak)) _fini(void)  {}

/* Functions */
__attribute__ ((used))
void initialise_monitor_handles()
{
}

__attribute__ ((used))
int _getpid(void)
{
        return 1;
}

__attribute__ ((used))
int _kill(int32_t pid, int32_t sig)
{
        errno = EINVAL;
        return -1;
}

__attribute__ ((used))
void _exit (int32_t status)
{
        _kill(status, -1);
        while (1) {}		/* Make sure we hang here */
}

__attribute__ ((used))
int _write(int32_t file, uint8_t *ptr, int32_t len)
{
        int32_t i;

        if(file == 1)
        {
                for(i = 0; i < len; i++)
                        fputc(ptr[i], stdout);
        }
        else if(file == 2)
        {
                for(i = 0; i < len; i++)
                        fputc(ptr[i], stderr);
        }

        return len;
}

__attribute__ ((used))
caddr_t _sbrk(int32_t incr)
{
        extern uint8_t end __asm("end");
        static uint8_t *heap_end;
        uint8_t *prev_heap_end;
        extern uint8_t _max_heap;

        if (heap_end == 0)
                heap_end = &end;

        prev_heap_end = heap_end;
        if (heap_end + incr > &_max_heap)
        {
//		write(1, "Heap and stack collision\n", 25);
//		abort();
                errno = ENOMEM;
                return (caddr_t) -1;
        }

        heap_end += incr;

        return (caddr_t) prev_heap_end;
}

__attribute__ ((used))
int _close(int32_t file)
{
        return -1;
}


__attribute__ ((used))
int _fstat(int32_t file, struct stat *st)
{
        st->st_mode = S_IFCHR;
        return 0;
}

int _isatty(int32_t file)
{
        return 1;
}

__attribute__ ((used))
int _lseek(int32_t file, int32_t ptr, int32_t dir)
{
        return 0;
}

__attribute__ ((used))
int _read(int32_t file, uint8_t *ptr, int32_t len)
{
        return 0;
}

__attribute__ ((used))
int _open(uint8_t *path, int32_t flags, ...)
{
        /* Pretend like we always fail */
        return -1;
}

__attribute__ ((used))
int _wait(int32_t *status)
{
        errno = ECHILD;
        return -1;
}

__attribute__ ((used))
int _unlink(uint8_t *name)
{
        errno = ENOENT;
        return -1;
}

__attribute__ ((used))
int _times(struct tms *buf)
{
        return -1;
}

__attribute__ ((used))
int _stat(uint8_t *file, struct stat *st)
{
        st->st_mode = S_IFCHR;
        return 0;
}

__attribute__ ((used))
int _link(uint8_t *old, uint8_t *new)
{
        errno = EMLINK;
        return -1;
}

__attribute__ ((used))
int _fork(void)
{
        errno = EAGAIN;
        return -1;
}

__attribute__ ((used))
int _execve(uint8_t *name, uint8_t **argv, uint8_t **env)
{
        errno = ENOMEM;
        return -1;
}

#pragma GCC diagnostic pop



//*************************************************************************//*****
// * @file     stdio.c
// * @brief    Implementation of newlib syscall
// ********************************************************************************/

//#include <stdio.h>
//#include <stdarg.h>
//#include <sys/types.h>
//#include <sys/stat.h>

//#undef errno
//extern int errno;
//extern int  _end;

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-parameter"

//void __attribute__ ((weak)) _fini(void)  {}

///*This function is used for handle heap option*/
//__attribute__ ((used))
//caddr_t _sbrk ( int incr )
//{
//    static unsigned char *heap = NULL;
//    unsigned char *prev_heap;

//    if (heap == NULL) {
//        heap = (unsigned char *)&_end;
//    }
//    prev_heap = heap;

//    heap += incr;

//    return (caddr_t) prev_heap;
//}

//__attribute__ ((used))
//int link(char *old, char *new)
//{
//    return -1;
//}

//__attribute__ ((used))
//int _close(int file)
//{
//    return -1;
//}

//__attribute__ ((used))
//int _fstat(int file, struct stat *st)
//{
//    st->st_mode = S_IFCHR;
//    return 0;
//}

//__attribute__ ((used))
//int _isatty(int file)
//{
//    return 1;
//}

//__attribute__ ((used))
//int _lseek(int file, int ptr, int dir)
//{
//    return 0;
//}

///*Low layer read(input) function*/
//__attribute__ ((used))
//int _read(int file, char *ptr, int len)
//{

//#if 0
//     //user code example
//     int i;
//     (void)file;

//     for(i = 0; i < len; i++)
//     {
//        // UART_GetChar is user's basic input function
//        *ptr++ = UART_GetChar();
//     }

//#endif

//    return len;
//}


///*Low layer write(output) function*/
//__attribute__ ((used))
//int _write(int file, char *ptr, int len)
//{

//#if 0
//     //user code example

//     int i;
//     (void)file;

//     for(i = 0; i < len; i++)
//     {
//        // UART_PutChar is user's basic output function
//        UART_PutChar(*ptr++);
//     }
//#endif

//    return len;
//}

//__attribute__ ((used))
//void abort(void)
//{
//    /* Abort called */
//    while(1);
//}

//#pragma GCC diagnostic pop
///* --------------------------------- End Of File ------------------------------
