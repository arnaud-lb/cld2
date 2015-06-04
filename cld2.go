// Package cld2 implements language detection using the
// Compact Language Detector.
//
// This package includes the relevant sources from the cld2
// project, so it doesn't require any external dependencies.
// For more information about CLD2, see https://code.google.com/p/cld2/.
package cld2

// #include <stdlib.h>
// #include "cld2.h"
import "C"
import "unsafe"

type Results struct {
	Language   string
	Summary    [3]Result
	TextBytes  int
	IsReliable bool
}

type Result struct {
	Language string
	Percent  int
}

type Hints struct {
	tld      string
	language string
}

func (h *Hints) SetTLD(tld string) {
	h.tld = tld
}

func (h *Hints) SetLanguageString(language string) {
	h.language = language
}

// Detect returns the language code for detected language
// in the given text.
func Detect(text string) string {
	cs := C.CString(text)
	res := C.DetectLang(cs, -1)
	C.free(unsafe.Pointer(cs))
	var lang string
	if res != nil {
		lang = C.GoString(res)
	}
	return lang
}

func DetectSummary(text string, isPlainText bool, allowExtendedLanguages bool, hints *Hints) *Results {

	cText := C.CString(text)
	defer C.free(unsafe.Pointer(cText))

	var cTLDHint, cLanguageHint *C.char

	if hints.tld != "" {
		cTLDHint = C.CString(hints.tld)
		defer C.free(unsafe.Pointer(cTLDHint))
	}

	if hints.language != "" {
		cLanguageHint = C.CString(hints.language)
		defer C.free(unsafe.Pointer(cLanguageHint))
	}

	var language3 [3]*C.char
	var percent3 [3]C.int
	var textBytes C.int
	var isReliable C.int

	summaryLanguage := C.DetectLangSummary(
		cText,
		C.int(len(text)),
		boolToCInt(isPlainText),
		boolToCInt(allowExtendedLanguages),
		cTLDHint,
		cLanguageHint,
		(**C.char)(unsafe.Pointer(&language3[0])),
		(*C.int)(unsafe.Pointer(&percent3[0])),
		&textBytes,
		&isReliable,
	)

	results := &Results{
		Language:   C.GoString(summaryLanguage),
		IsReliable: cIntToBool(isReliable),
		TextBytes:  int(textBytes),
	}

	for i := 0; i < 3; i++ {
		results.Summary[i].Language = C.GoString(language3[i])
		results.Summary[i].Percent = int(percent3[i])
	}

	return results
}

func boolToCInt(b bool) C.int {
	if b {
		return 0
	}
	return 1
}

func cIntToBool(i C.int) bool {
	if i == 0 {
		return false
	}
	return true
}
