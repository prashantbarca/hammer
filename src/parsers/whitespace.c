#include <ctype.h>
#include "parser_internal.h"

static HParseResult* parse_whitespace(void* env, HParseState *state) {
  char c;
  HInputStream bak;
  do {
    bak = state->input_stream;
    c = h_read_bits(&state->input_stream, 8, false);
    if (state->input_stream.overrun)
      return NULL;
  } while (isspace(c));
  state->input_stream = bak;
  return h_do_parse((HParser*)env, state);
}

static bool ws_isValidRegular(void *env) {
  HParser *p = (HParser*)env;
  return p->vtable->isValidRegular(p->env);
}

static bool ws_isValidCF(void *env) {
  HParser *p = (HParser*)env;
  return p->vtable->isValidCF(p->env);
}

static bool ws_ctrvm(HRVMProg *prog, void *env) {
  HParser *p = (HParser*)env;
  return h_compile_regex(prog, p->env);
}

static const HParserVtable whitespace_vt = {
  .parse = parse_whitespace,
  .isValidRegular = ws_isValidRegular,
  .isValidCF = ws_isValidCF,
  .compile_to_rvm = ws_ctrvm,
};

const HParser* h_whitespace(const HParser* p) {
  return h_whitespace__m(&system_allocator, p);
}
const HParser* h_whitespace__m(HAllocator* mm__, const HParser* p) {
  HParser *ret = h_new(HParser, 1);
  ret->vtable = &whitespace_vt;
  ret->env = (void*)p;
  return ret;
}
