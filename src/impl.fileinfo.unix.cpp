/* /////////////////////////////////////////////////////////////////////////
 * File:        impl.fileinfo.unix.cpp
 *
 * Purpose:     UNIX implementation for the file information blocks of the recls API.
 *
 * Created:     2nd November 2003
 * Updated:     10th January 2017
 *
 * Home:        http://recls.org/
 *
 * Copyright (c) 2003-2017, Matthew Wilson and Synesis Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - Neither the name(s) of Matthew Wilson and Synesis Software nor the
 *   names of any contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////// */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <recls/recls.h>
#include <recls/assert.h>
#include "impl.root.h"
#include "incl.unixstl.h"
#include "impl.cover.h"

#include "impl.trace.h"

#include <stlsoft/synch/lock_scope.hpp>

//#define   RECLS_UNIX_USE_ATOMIC_OPERATIONS // Define this if you're on Linux (and you know what you're doing!)

#if defined(RECLS_MT)
 // If we're multi-threading, then we have two options:
# if defined(RECLS_UNIX_USE_ATOMIC_OPERATIONS)
  // 1. On Linux we can use the kernel's atomic operations, since all we need 
  //    is atomic integer operations. Since these kernel operations are not
  //    standard, you must explicitly select them in your build by defining
  //    the symbol RECLS_UNIX_USE_ATOMIC_OPERATIONS
#  include <asm/atomic.h>
# else /* ? RECLS_UNIX_USE_ATOMIC_OPERATIONS */
  // 2. On other UNIX systems we use the UNIXSTL thread_mutex class
#  include <unixstl/synch/thread_mutex.hpp>
# endif /* !RECLS_UNIX_USE_ATOMIC_OPERATIONS */
#else /* ? RECLS_MT */
 // When not multi-threaded, we just use the STLSoft null_mutex class, which
 // is just a stub
# include <stlsoft/synch/null_mutex.hpp>
#endif /* RECLS_MT */


#include "impl.atomic.h"

/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(RECLS_NO_NAMESPACE)
namespace recls
{
namespace impl
{
#endif /* !RECLS_NO_NAMESPACE */

/* /////////////////////////////////////////////////////////////////////////
 * statics
 */

#if !defined(RECLS_UNIX_USE_ATOMIC_OPERATIONS)
namespace
{
#if defined(RECLS_MT)
    unixstl::thread_mutex   s_mx(true);

    typedef ::unixstl::thread_mutex mutex_t;
#else /* ? RECLS_MT */
    ::stlsoft::null_mutex   s_mx;

    typedef ::stlsoft::null_mutex   mutex_t;
#endif /* RECLS_MT */
}
#endif /* !RECLS_UNIX_USE_ATOMIC_OPERATIONS */

/* /////////////////////////////////////////////////////////////////////////
 * helpers
 */

RECLS_FNDECL(void) RC_Increment(rc_atomic_t volatile* p)
{
    RECLS_COVER_MARK_LINE();

#if defined(RECLS_UNIX_USE_ATOMIC_OPERATIONS)
    atomic_inc(p);
#else /* ? RECLS_UNIX_USE_ATOMIC_OPERATIONS */
    ::stlsoft::lock_scope<mutex_t>        lock(s_mx);

    ++(*p);
#endif /* !RECLS_UNIX_USE_ATOMIC_OPERATIONS */
}

RECLS_FNDECL(rc_atomic_t) RC_PreDecrement(rc_atomic_t volatile* p)
{
    RECLS_COVER_MARK_LINE();

#if defined(RECLS_UNIX_USE_ATOMIC_OPERATIONS)
    return 1 + atomic_dec_and_test(p);
#else /* ? RECLS_UNIX_USE_ATOMIC_OPERATIONS */
    ::stlsoft::lock_scope<mutex_t>        lock(s_mx);

    return --(*p);
#endif /* !RECLS_UNIX_USE_ATOMIC_OPERATIONS */
}

RECLS_FNDECL(rc_atomic_t) RC_ReadValue(rc_atomic_t volatile* p)
{
    RECLS_COVER_MARK_LINE();

#if defined(RECLS_UNIX_USE_ATOMIC_OPERATIONS)
    return atomic_read(p);
#else /* ? RECLS_UNIX_USE_ATOMIC_OPERATIONS */
    ::stlsoft::lock_scope<mutex_t>        lock(s_mx);

    return (*p);
#endif /* !RECLS_UNIX_USE_ATOMIC_OPERATIONS */
}

/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(RECLS_NO_NAMESPACE)
} /* namespace impl */
} /* namespace recls */
#endif /* !RECLS_NO_NAMESPACE */

/* ///////////////////////////// end of file //////////////////////////// */
