/* /////////////////////////////////////////////////////////////////////////
 * File:        ReclsSearch.cpp
 *
 * Purpose:     Implementation of the ReclsFileSearch class for Windows.
 *
 * Created:     16th August 2003
 * Updated:     29th January 2017
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
#include "impl.util.h"
#include "impl.cover.h"

#include "impl.trace.h"

#include "ReclsSearch.hpp"

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
 * coverage
 */

RECLS_ASSOCIATE_FILE_WITH_CORE_GROUP()
RECLS_ASSOCIATE_FILE_WITH_GROUP("recls.core.search")
RECLS_MARK_FILE_START()

/* /////////////////////////////////////////////////////////////////////////
 * ReclsSearch
 */

ReclsSearch::ReclsSearch()
    : m_dnode(NULL)
    , m_lastError(RECLS_RC_OK)
{
    RECLS_COVER_MARK_LINE();
}

ReclsSearch::~ReclsSearch()
{
    RECLS_COVER_MARK_LINE();

    delete m_dnode;
}

/* static */ hrecls_t ReclsSearch::ToHandle(ReclsSearch* si)
{
    RECLS_COVER_MARK_LINE();

    return hrecls_t(si);
}

/* static */ ReclsSearch* ReclsSearch::FromHandle(hrecls_t h)
{
    RECLS_COVER_MARK_LINE();

    return static_cast<ReclsSearch*>(const_cast<void*>(static_cast<void const*>(h)));
}

// Operations

recls_rc_t ReclsSearch::GetNext()
{
    function_scope_trace("ReclsSearch::GetNext");

    RECLS_ASSERT(NULL != m_dnode);

    RECLS_COVER_MARK_LINE();

    m_lastError = m_dnode->GetNext();

    if(RECLS_RC_NO_MORE_DATA == m_lastError)
    {
        RECLS_COVER_MARK_LINE();

        delete m_dnode;

        m_dnode = NULL;
    }

    return m_lastError;
}

recls_rc_t ReclsSearch::GetDetails(recls_entry_t* pinfo)
{
    function_scope_trace("ReclsSearch::GetDetails");

    RECLS_ASSERT(NULL != m_dnode);

    RECLS_COVER_MARK_LINE();

    return (m_lastError = m_dnode->GetDetails(pinfo));
}

recls_rc_t ReclsSearch::GetNextDetails(recls_entry_t* pinfo)
{
    function_scope_trace("ReclsSearch::GetNextDetails");

    RECLS_ASSERT(NULL != m_dnode);

    RECLS_COVER_MARK_LINE();

    m_lastError = m_dnode->GetNextDetails(pinfo);

    if(RECLS_RC_NO_MORE_DATA == m_lastError)
    {
        RECLS_COVER_MARK_LINE();

        delete m_dnode;

        m_dnode = NULL;
    }

    return m_lastError;
}

// Accessors

recls_rc_t ReclsSearch::GetLastError() const
{
    function_scope_trace("ReclsSearch::GetLastError");

    RECLS_COVER_MARK_LINE();

    return m_lastError;
}

/* /////////////////////////////////////////////////////////////////////////
 * coverage
 */

RECLS_MARK_FILE_END()

/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(RECLS_NO_NAMESPACE)
} /* namespace impl */
} /* namespace recls */
#endif /* !RECLS_NO_NAMESPACE */

/* ///////////////////////////// end of file //////////////////////////// */
