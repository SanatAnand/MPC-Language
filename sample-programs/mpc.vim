" Vim syntax file
" Language:	MPC
" Adapted from C syntax

" Quit when a (custom) syntax file was already loaded
if exists("b:current_syntax")
  finish
endif

let s:cpo_save = &cpo
set cpo&vim

let s:ft = matchstr(&ft, '^\([^.]\)\+')

" keywords, special operators
syn keyword	mpcStatement	in set start partial open as of from where except connect forward to and abort with
syn keyword	mpcConditional	if else
syn keyword	mpcRepeat	for each

syn keyword	mpcType		int bool unknown party session protocol inport outport port field local public
syn keyword	mpcType		environ random

syn keyword	mpcSpecialStatement	uses  wrap before after reading writing
syn keyword	mpcStructure	struct enum
syn keyword	mpcStorageClass	const
syn keyword	mpcConstant true false
syn region	mpcCppSkip	contained start="^\s*\(%:\|#\)\s*\(if\>\|ifdef\>\|ifndef\>\)" skip="\\$" end="^\s*\(%:\|#\)\s*endif\>" contains=mpcSpaceError,mpcCppSkip

syn match	mpcOperator	"=>"
syn match	mpcSpecialOperator display "#" 
" syn match	mpcSpecialOperator display "@"

" It's easy to accidentally add a space after a backslash that was intended
" for line continuation.  Some compilers allow it, which makes it
" unpredictable and should be avoided.
syn match	mpcBadContinuation contained "\\\s\+$"

" mpcCommentGroup allows adding matches for special things in comments
syn cluster	mpcCommentGroup	contains=mpcTodo,mpcBadContinuation

" String and Character constants
" Highlight special characters (those which have a backslash) differently
syn match	mpcSpecial	display contained "\\\(x\x\+\|\o\{1,3}\|.\|$\)"
syn match	mpcSpecial	display contained "\\\(u\x\{4}\|U\x\{8}\)"


syn cluster	mpcStringGroup	contains=mpcCppString,mpcCppSkip
syn region	mpcString		start=+L\="+ skip=+\\\\\|\\"+ end=+"+ contains=mpcSpecial,@Spell extend

syn match	mpcCharacter	"L\='[^\\]'"
syn match	mpcCharacter	"L'[^']*'" contains=mpcSpecial
syn match	mpcSpecialError	"L\='\\[^'\"?\\abefnrtv]'"
syn match	mpcSpecialCharacter "L\='\\['\"?\\abefnrtv]'"
syn match	mpcSpecialCharacter display "L\='\\\o\{1,3}'"
syn match	mpcSpecialCharacter display "'\\x\x\{1,2}'"
syn match	mpcSpecialCharacter display "L'\\x\x\+'"

syn match	mpcCharacter	"[Uu]'[^\\]'"
syn match	mpcCharacter	"[Uu]'[^']*'" contains=mpcSpecial
syn match	mpcSpecialError	"[Uu]'\\[^'\"?\\abefnrtv]'"
syn match	mpcSpecialCharacter "[Uu]'\\['\"?\\abefnrtv]'"
syn match	mpcSpecialCharacter display "[Uu]'\\\o\{1,3}'"
syn match	mpcSpecialCharacter display "[Uu]'\\x\x\+'"

" This should be before mpcErrInParen to avoid problems with #define ({ xxx })
syn match mpcCurlyError "}"
syn region	mpcBlock		start="{" end="}" contains=ALLBUT,mpcBadBlock,mpcCurlyError,@mpcParenGroup,mpcErrInParen,mpcCppParen,mpcErrInBracket,mpcCppBracket,@mpcStringGroup,@Spell fold

" Catch errors caused by wrong parenthesis and brackets.
" Also accept <% for {, %> for }, <: for [ and :> for ] (C99)
" But avoid matching <::.
syn cluster	mpcParenGroup	contains=mpcParenError,mpcIncluded,mpcSpecial,mpcCommentSkip,mpcCommentString,mpcComment2String,@mpcCommentGroup,mpcCommentStartError,mpcOctalZero,@mpcCppOutInGroup,mpcNumber,mpcFloat,mpcOctal,mpcOctalError,mpcNumbersCom
syn region	mpcParen		transparent start='(' end=')' end='}'me=s-1 contains=ALLBUT,mpcBlock,@mpcParenGroup,mpcCppParen,@mpcStringGroup,@Spell
" mpcCppParen: same as mpcParen but ends at end-of-line; used in mpcDefine
syn region	mpcCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@mpcParenGroup,mpcParen,mpcString,@Spell
syn match	mpcParenError	display ")"
syn match	mpcErrInParen	display contained "^[{}]\|^<%\|^%>"

syn region	mpcBadBlock	keepend start="{" end="}" contained containedin=mpcParen,mpcBracket,mpcBadBlock transparent fold

"integer number, or floating point number without a dot and with "f".
syn case ignore
syn match	mpcNumbers	display transparent "\<\d\|\.\d" contains=mpcNumber,mpcFloat,mpcOctalError,mpcOctal
" Same, but without octal error (for comments)
syn match	mpcNumbersCom	display contained transparent "\<\d\|\.\d" contains=mpcNumber,mpcFloat,mpcOctal
syn match	mpcNumber		display contained "\d\+\(u\=l\{0,2}\|ll\=u\)\>"
"hex number
syn match	mpcNumber		display contained "0x\x\+\(u\=l\{0,2}\|ll\=u\)\>"
" Flag the first zero of an octal number as something special
syn match	mpcOctal		display contained "0\o\+\(u\=l\{0,2}\|ll\=u\)\>" contains=mpcOctalZero
syn match	mpcOctalZero	display contained "\<0"
syn match	mpcFloat		display contained "\d\+f"
"floating point number, with dot, optional exponent
syn match	mpcFloat		display contained "\d\+\.\d*\(e[-+]\=\d\+\)\=[fl]\="
"floating point number, starting with a dot, optional exponent
syn match	mpcFloat		display contained "\.\d\+\(e[-+]\=\d\+\)\=[fl]\=\>"
"floating point number, without dot, with exponent
syn match	mpcFloat		display contained "\d\+e[-+]\=\d\+[fl]\=\>"

" flag an octal number with wrong digits
syn match	mpcOctalError	display contained "0\o*[89]\d*"
syn case match

if exists("c_comment_strings")
  " A comment can contain mpcString, mpcCharacter and mpcNumber.
  " But a "*/" inside a mpcString in a mpcComment DOES end the comment!  So we
  " need to use a special type of mpcString: mpcCommentString, which also ends on
  " "*/", and sees a "*" at the start of the line as comment again.
  " Unfortunately this doesn't very well work for // type of comments :-(
  syn match	mpcCommentSkip	contained "^\s*\*\($\|\s\+\)"
  syn region mpcCommentString	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end=+\*/+me=s-1 contains=mpcSpecial,mpcCommentSkip
  syn region mpcComment2String	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end="$" contains=mpcSpecial
  syn region  mpcCommentL	start="//" skip="\\$" end="$" keepend contains=@mpcCommentGroup,mpcComment2String,mpcCharacter,mpcNumbersCom,mpcSpaceError,@Spell
  if exists("c_no_comment_fold")
    " Use "extend" here to have preprocessor lines not terminate halfway a
    " comment.
    syn region mpcComment	matchgroup=mpcCommentStart start="/\*" end="\*/" contains=@mpcCommentGroup,mpcCommentStartError,mpcCommentString,mpcCharacter,mpcNumbersCom,mpcSpaceError,@Spell extend
  else
    syn region mpcComment	matchgroup=mpcCommentStart start="/\*" end="\*/" contains=@mpcCommentGroup,mpcCommentStartError,mpcCommentString,mpcCharacter,mpcNumbersCom,mpcSpaceError,@Spell fold extend
  endif
else
  syn region	mpcCommentL	start="//" skip="\\$" end="$" keepend contains=@mpcCommentGroup,mpcSpaceError,@Spell
  if exists("c_no_comment_fold")
    syn region	mpcComment	matchgroup=mpcCommentStart start="/\*" end="\*/" contains=@mpcCommentGroup,mpcCommentStartError,mpcSpaceError,@Spell extend
  else
    syn region	mpcComment	matchgroup=mpcCommentStart start="/\*" end="\*/" contains=@mpcCommentGroup,mpcCommentStartError,mpcSpaceError,@Spell fold extend
  endif
endif
" keep a // comment separately, it terminates a preproc. conditional
syn match	mpcCommentError	display "\*/"
syn match	mpcCommentStartError display "/\*"me=e-1 contained

syn keyword	mpcTodo		contained TODO FIXME XXX

syn sync fromstart

" Define the default highlighting.
" Only used when an item doesn't have highlighting yet
hi def link mpcCppString	mpcString
hi def link mpcCommentL		mpcComment
hi def link mpcCommentStart	mpcComment
hi def link mpcLabel		Label
hi def link mpcConditional	Conditional
hi def link mpcRepeat		Repeat
hi def link mpcCharacter	Character
hi def link mpcSpecialCharacter	mpcSpecial
hi def link mpcNumber		Number
hi def link mpcOctal		Number
hi def link mpcOctalZero	PreProc	 " link this to Error if you want
hi def link mpcFloat		Float
hi def link mpcOctalError	mpcError
hi def link mpcParenError	mpcError
hi def link mpcErrInParen	mpcError
hi def link mpcErrInBracket	mpcError
hi def link mpcCommentError	mpcError
hi def link mpcCommentStartError	mpcError
hi def link mpcSpaceError	mpcError
hi def link mpcSpecialError	mpcError
hi def link mpcCurlyError	mpcError
hi def link mpcOperator		Operator
hi def link mpcSpecialOperator	PreProc
hi def link mpcStructure	Structure
hi def link mpcStorageClass	StorageClass
hi def link mpcInclude		Include
hi def link mpcPreProc		PreProc
hi def link mpcDefine		Macro
hi def link mpcIncluded		mpcString
hi def link mpcError		Error
hi def link mpcStatement	Statement
hi def link mpcType		Type
hi def link mpcConstant		Constant
hi def link mpcCommentString	mpcString
hi def link mpcComment2String	mpcString
hi def link mpcCommentSkip	mpcComment
hi def link mpcString		String
hi def link mpcComment		Comment
hi def link mpcSpecial		SpecialChar
hi def link mpcSpecialStatement		SpecialChar
hi def link mpcTodo		Todo
hi def link mpcBadContinuation	Error

let b:current_syntax = "mpc"

unlet s:ft

let &cpo = s:cpo_save
unlet s:cpo_save
" vim: ts=8
