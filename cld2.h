#ifdef __cplusplus
extern "C" {
#endif

const char* DetectLang(char *data, int length);
const char* DetectLangSummary(char *data, int length, int is_plain_text, int allow_extended_lang, const char *tld_hint, const char *language_hint, const char **language3, int *percent3, int *text_bytes, int *is_reliable);

#ifdef __cplusplus
}
#endif
