#include <cstddef>
#include <string.h>
#include <stdio.h>
#include <string>

#include "compact_lang_det.h"
#include "cld2.h"

const char* DetectLang(char *data, int length) {

    bool is_plain_text = true;
    CLD2::CLDHints cldhints = {NULL, NULL, 0, CLD2::UNKNOWN_LANGUAGE};
    bool allow_extended_lang = true;
    int flags = 0;
    CLD2::Language language3[3];
    int percent3[3];
    double normalized_score3[3];
    CLD2::ResultChunkVector resultchunkvector;
    int text_bytes;
    bool is_reliable;

    if (length <= 0) {
        length = strlen(data);
    }

    CLD2::Language summary_lang = CLD2::UNKNOWN_LANGUAGE;

    summary_lang = CLD2::ExtDetectLanguageSummary(data, 
            length,
            is_plain_text,
            &cldhints,
            flags,
            language3,
            percent3,
            normalized_score3,
            &resultchunkvector,
            &text_bytes,
            &is_reliable);

    return CLD2::LanguageCode(summary_lang);
}

const char* DetectLangSummary(char *data, int length, int is_plain_text, int allow_extended_lang, const char *tld_hint, const char *language_hint, const char **language3, int *percent3, int *text_bytes, int *is_reliable) {

    CLD2::CLDHints cldhints = {NULL, NULL, 0, CLD2::UNKNOWN_LANGUAGE};
    int flags = 0;
    CLD2::Language cld_language3[3];
    double normalized_score3[3];
    CLD2::ResultChunkVector resultchunkvector;
    bool cld_is_reliable;
    int i;

    if (length <= 0) {
        length = strlen(data);
    }

    if (language_hint) {
        cldhints.language_hint = CLD2::GetLanguageFromName(language_hint);
    }

    CLD2::Language summary_lang = CLD2::UNKNOWN_LANGUAGE;

    summary_lang = CLD2::ExtDetectLanguageSummary(data, 
            length,
            (bool) is_plain_text,
            &cldhints,
            flags,
            cld_language3,
            percent3,
            normalized_score3,
            &resultchunkvector,
            text_bytes,
            &cld_is_reliable);

    for (i = 0; i < 3; i++) {
        language3[i] = CLD2::LanguageCode(cld_language3[i]);
    }

    *is_reliable = cld_is_reliable;

    return CLD2::LanguageCode(summary_lang);
}
