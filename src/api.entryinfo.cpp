/* /////////////////////////////////////////////////////////////////////////
 * File:        api.entryinfo.cpp
 *
 * Purpose:     recls API functions pertaining to entry info.
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
#include "incl.platformstl.h"
#include "impl.util.h"
#include "impl.cover.h"

#include "impl.trace.h"

#if defined(RECLS_PLATFORM_IS_UNIX)
# include <sys/stat.h>
#endif /* UNIX */

/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(RECLS_NO_NAMESPACE)
namespace recls
{

using ::recls::impl::recls_get_string_property_;
using ::recls::impl::recls_file_exists_;
#endif /* !RECLS_NO_NAMESPACE */

namespace
{

#ifdef STLSOFT_ALWAYS_FALSE
# define always_false_()                                    STLSOFT_ALWAYS_FALSE()
#else
static
int
always_false_()
{
    return 0;
}
#endif

} /* anonymous namespace */

/* /////////////////////////////////////////////////////////////////////////
 * coverage
 */

RECLS_ASSOCIATE_FILE_WITH_CORE_GROUP()
RECLS_ASSOCIATE_FILE_WITH_GROUP("recls.core.search")
RECLS_MARK_FILE_START()

/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#if defined(RECLS_PLATFORM_IS_UNIX)
# define GetCreationTime_           modificationTime
# define GetLastStatusChangeTime_   lastStatusChangeTime
#elif defined(RECLS_PLATFORM_IS_WINDOWS)
# define GetCreationTime_           creationTime
# define GetLastStatusChangeTime_   modificationTime
#else /* unrecognised platform */
# error platform is not recognised
#endif /* platform */

/* /////////////////////////////////////////////////////////////////////////
 * file entry info structure
 */

RECLS_FNDECL(size_t) Recls_GetPathProperty(
    recls_entry_t   fileInfo
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetPathProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return recls_get_string_property_(&fileInfo->path, buffer, cchBuffer);
}

RECLS_FNDECL(size_t) Recls_GetSearchRelativePathProperty(
    recls_entry_t   fileInfo
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetSearchRelativePathProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return recls_get_string_property_(&fileInfo->searchRelativePath, buffer, cchBuffer);
}

RECLS_FNDECL(size_t) Recls_GetDirectoryProperty(
    recls_entry_t   fileInfo
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetDirectoryProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return recls_get_string_property_(&fileInfo->directory, buffer, cchBuffer);
}

RECLS_FNDECL(size_t) Recls_GetDirectoryPathProperty(
    recls_entry_t   fileInfo
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetDirectoryPathProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    struct recls_strptrs_t directoryPath =
    {
            fileInfo->path.begin    /* Directory path is defined by start of path ... */
        ,   fileInfo->directory.end /* ... to end of directory. */
    };

    return recls_get_string_property_(&directoryPath, buffer, cchBuffer);
}

RECLS_FNDECL(size_t) Recls_GetSearchDirectoryProperty(
    recls_entry_t   fileInfo
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetSearchDirectoryProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return recls_get_string_property_(&fileInfo->searchDirectory, buffer, cchBuffer);
}

RECLS_FNDECL(size_t) Recls_GetUNCDriveProperty(
    recls_entry_t   fileInfo
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetUNCDriveProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    struct recls_strptrs_t uncDrive =
    {
            fileInfo->path.begin        /* Directory path is defined by start of path ... */
        ,   fileInfo->directory.begin   /* ... to start of directory. */
    };

    if(!Recls_IsFileUNC(fileInfo))
    {
        RECLS_COVER_MARK_LINE();

        uncDrive.end = uncDrive.begin;
    }

    return recls_get_string_property_(&uncDrive, buffer, cchBuffer);
}

RECLS_FNDECL(size_t) Recls_GetFileProperty(
    recls_entry_t   fileInfo
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetFileProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    struct recls_strptrs_t file =
    {
            fileInfo->fileName.begin /* File is defined by start of fileName ... */
        ,   fileInfo->fileExt.end    /* ... to end of fileExt. */
    };

    return recls_get_string_property_(&file, buffer, cchBuffer);
}

RECLS_FNDECL(size_t) Recls_GetFileNameProperty(
    recls_entry_t   fileInfo
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetFileNameProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return recls_get_string_property_(&fileInfo->fileName, buffer, cchBuffer);
}

RECLS_FNDECL(size_t) Recls_GetFileExtProperty(
    recls_entry_t   fileInfo
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetFileExtProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return recls_get_string_property_(&fileInfo->fileExt, buffer, cchBuffer);
}

RECLS_FNDECL(size_t) Recls_GetDirectoryPartProperty(
    recls_entry_t   fileInfo
,   int             part
,   recls_char_t*   buffer
,   size_t          cchBuffer
)
{
    function_scope_trace("Recls_GetDirectoryPartProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    size_t  cParts = static_cast<size_t>(fileInfo->directoryParts.end - fileInfo->directoryParts.begin);

    if(part < 0)
    {
        RECLS_COVER_MARK_LINE();

        return cParts;
    }
    else
    {
        RECLS_ASSERT(static_cast<size_t>(part) < cParts);

        RECLS_COVER_MARK_LINE();

        return recls_get_string_property_(&fileInfo->directoryParts.begin[part], buffer, cchBuffer);
    }
}

RECLS_FNDECL(recls_bool_t) Recls_EntryExists(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_EntryExists");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    if( 0 != fileInfo->size ||
        0 != fileInfo->attributes ||
#if defined(RECLS_PLATFORM_IS_WINDOWS)
        0 != fileInfo->GetCreationTime_.dwLowDateTime ||
        0 != fileInfo->GetCreationTime_.dwHighDateTime ||
        0 != fileInfo->GetLastStatusChangeTime_.dwLowDateTime ||
        0 != fileInfo->GetLastStatusChangeTime_.dwHighDateTime ||
        0 != fileInfo->modificationTime.dwLowDateTime ||
        0 != fileInfo->modificationTime.dwHighDateTime ||
#else
        0 != fileInfo->GetCreationTime_ ||
        0 != fileInfo->GetLastStatusChangeTime_ ||
        0 != fileInfo->modificationTime ||
#endif
        always_false_())
    {
        RECLS_COVER_MARK_LINE();

        return true;
    }

    RECLS_COVER_MARK_LINE();

    return false;
}

RECLS_FNDECL(recls_bool_t) Recls_IsFileReadOnly(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_IsFileReadOnly");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

#if defined(RECLS_PLATFORM_IS_UNIX)
    return (fileInfo->attributes & S_IWRITE) == 0;
#elif defined(RECLS_PLATFORM_IS_WINDOWS)
    return (fileInfo->attributes & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY;
#else /* unrecognised platform */
# error platform is not recognised
#endif /* platform */
}

RECLS_FNDECL(recls_bool_t) Recls_IsFileDirectory(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_IsFileDirectory");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

#if defined(RECLS_PLATFORM_IS_UNIX)
    return (fileInfo->attributes & S_IFMT) == S_IFDIR;
#elif defined(RECLS_PLATFORM_IS_WINDOWS)
    return (fileInfo->attributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
#else /* unrecognised platform */
# error platform is not recognised
#endif /* platform */
}

RECLS_FNDECL(recls_bool_t) Recls_IsFileLink(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_IsFileLink");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

#if defined(RECLS_PLATFORM_IS_UNIX) && \
    !defined(RECLS_PLATFORM_IS_UNIX_EMULATED_ON_WINDOWS)

    return (fileInfo->attributes & S_IFMT) == S_IFLNK;

#else /* unrecognised platform */

    STLSOFT_SUPPRESS_UNUSED(fileInfo);

    return false;

#endif /* platform */
}

RECLS_FNDECL(recls_bool_t) Recls_DoesEntryExist(recls_entry_t hEntry)
{
    function_scope_trace("Recls_DoesEntryExist");

    RECLS_ASSERT(NULL != hEntry);

    RECLS_COVER_MARK_LINE();

    return recls_file_exists_(hEntry->path.begin);
}

RECLS_FNDECL(recls_bool_t) Recls_IsFileUNC(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_IsFileUNC");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

#if defined(RECLS_PLATFORM_IS_WINDOWS) || \
    defined(RECLS_PLATFORM_IS_UNIX_EMULATED_ON_WINDOWS)

    if( '\\' == fileInfo->path.begin[0] &&
        '\\' == fileInfo->path.begin[1])
    {
        RECLS_COVER_MARK_LINE();

        return true;
    }

#else /* unrecognised platform */

    STLSOFT_SUPPRESS_UNUSED(fileInfo);

#endif /* platform */

    return false;
}

RECLS_FNDECL(recls_time_t) Recls_GetCreationTime(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_GetCreationTime");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return fileInfo->GetCreationTime_;
}

RECLS_FNDECL(recls_time_t) Recls_GetLastStatusChangeTime(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_GetLastStatusChangeTime");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return fileInfo->GetLastStatusChangeTime_;
}

RECLS_FNDECL(recls_filesize_t) Recls_GetSizeProperty(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_GetSizeProperty");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return fileInfo->size;
}

RECLS_FNDECL(recls_time_t) Recls_GetModificationTime(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_GetModificationTime");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return fileInfo->modificationTime;
}

RECLS_FNDECL(recls_time_t) Recls_GetLastAccessTime(recls_entry_t fileInfo)
{
    function_scope_trace("Recls_GetLastAccessTime");

    RECLS_ASSERT(NULL != fileInfo);

    RECLS_COVER_MARK_LINE();

    return fileInfo->lastAccessTime;
}

/* /////////////////////////////////////////////////////////////////////////
 * coverage
 */

RECLS_MARK_FILE_END()

/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

#if !defined(RECLS_NO_NAMESPACE)
} /* namespace recls */
#endif /* !RECLS_NO_NAMESPACE */

/* ///////////////////////////// end of file //////////////////////////// */
