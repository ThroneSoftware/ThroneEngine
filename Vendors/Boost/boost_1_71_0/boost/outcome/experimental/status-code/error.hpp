/* Proposed SG14 status_code
(C) 2018-2019 Niall Douglas <http://www.nedproductions.biz/> (5 commits)
File Created: Feb 2018


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef BOOST_OUTCOME_SYSTEM_ERROR2_ERROR_HPP
#define BOOST_OUTCOME_SYSTEM_ERROR2_ERROR_HPP

#include "errored_status_code.hpp"
#include "system_code.hpp"

BOOST_OUTCOME_SYSTEM_ERROR2_NAMESPACE_BEGIN

/*! An erased `system_code` which is always a failure. The closest equivalent to
`std::error_code`, except it cannot be null and cannot be modified.

This refines `system_code` into an `error` object meeting the requirements of
[P0709 Zero-overhead deterministic exceptions](https://wg21.link/P0709).

Differences from `system_code`:

- Always a failure (this is checked at construction, and if not the case,
the program is terminated as this is a logic error)
- No default construction.
- No empty state possible.
- Is immutable.

As with `system_code`, it remains guaranteed to be two CPU registers in size,
and move relocating.
*/
using error = errored_status_code<erased<system_code::value_type>>;

#ifndef NDEBUG
static_assert(sizeof(error) == 2 * sizeof(void *), "error is not exactly two pointers in size!");
static_assert(traits::is_move_relocating<error>::value, "error is not move relocating!");
#endif

BOOST_OUTCOME_SYSTEM_ERROR2_NAMESPACE_END

#endif
