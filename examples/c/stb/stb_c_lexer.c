#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_C_LEX_C_DECIMAL_INTS \
  Y  //  "0|[1-9][0-9]*"                        CLEX_intlit
#define STB_C_LEX_C_HEX_INTS \
  Y  //  "0x[0-9a-fA-F]+"                       CLEX_intlit
#define STB_C_LEX_C_OCTAL_INTS \
  Y  //  "[0-7]+"                               CLEX_intlit
#define STB_C_LEX_C_DECIMAL_FLOATS \
  Y  //  "[0-9]*(.[0-9]*([eE][-+]?[0-9]+)?)     CLEX_floatlit
#define STB_C_LEX_C99_HEX_FLOATS \
  Y  //  "0x{hex}+(.{hex}*)?[pP][-+]?{hex}+     CLEX_floatlit
#define STB_C_LEX_C_IDENTIFIERS \
  Y  //  "[_a-zA-Z][_a-zA-Z0-9]*"               CLEX_id
#define STB_C_LEX_C_DQ_STRINGS \
  Y  //  double-quote-delimited strings with escapes  CLEX_dqstring
#define STB_C_LEX_C_SQ_STRINGS \
  Y  //  single-quote-delimited strings with escapes  CLEX_ssstring
#define STB_C_LEX_C_CHARS \
  Y  //  single-quote-delimited character with escape CLEX_charlits
#define STB_C_LEX_C_COMMENTS Y    //  "/* comment */"
#define STB_C_LEX_CPP_COMMENTS Y  //  "// comment to end of line\n"
#define STB_C_LEX_C_COMPARISONS \
  Y  //  "==" CLEX_eq  "!=" CLEX_noteq   "<=" CLEX_lesseq  ">=" CLEX_greatereq
#define STB_C_LEX_C_LOGICAL Y     //  "&&"  CLEX_andand   "||"  CLEX_oror
#define STB_C_LEX_C_SHIFTS Y      //  "<<"  CLEX_shl      ">>"  CLEX_shr
#define STB_C_LEX_C_INCREMENTS Y  //  "++"  CLEX_plusplus "--"  CLEX_minusminus
#define STB_C_LEX_C_ARROW Y       //  "->"  CLEX_arrow
#define STB_C_LEX_EQUAL_ARROW Y   //  "=>"  CLEX_eqarrow
#define STB_C_LEX_C_BITWISEEQ \
  Y  //  "&="  CLEX_andeq    "|="  CLEX_oreq     "^="  CLEX_xoreq
#define STB_C_LEX_C_ARITHEQ \
  Y  //  "+="  CLEX_pluseq   "-="  CLEX_minuseq
     //  "*="  CLEX_muleq    "/="  CLEX_diveq    "%=" CLEX_modeq
     //  if both STB_C_LEX_SHIFTS & STB_C_LEX_ARITHEQ:
     //                      "<<=" CLEX_shleq    ">>=" CLEX_shreq

#define STB_C_LEX_PARSE_SUFFIXES \
  Y  // letters after numbers are parsed as part of those numbers, and must be
     // in suffix list below
#define STB_C_LEX_DECIMAL_SUFFIXES \
  "uUlL"  // decimal integer suffixes e.g. "uUlL" -- these are returned as-is in
          // string storage
#define STB_C_LEX_HEX_SUFFIXES "lL"      // e.g. "uUlL"
#define STB_C_LEX_OCTAL_SUFFIXES "lL"    // e.g. "uUlL"
#define STB_C_LEX_FLOAT_SUFFIXES "uulL"  //

#define STB_C_LEX_0_IS_EOF \
  N  // if Y, ends parsing at '\0'; if N, returns '\0' as token
#define STB_C_LEX_INTEGERS_AS_DOUBLES \
  N  // parses integers as doubles so they can be larger than 'int', but only if
     // STB_C_LEX_STDLIB==N
#define STB_C_LEX_MULTILINE_DSTRINGS \
  Y  // allow newlines in double-quoted strings
#define STB_C_LEX_MULTILINE_SSTRINGS \
  Y  // allow newlines in single-quoted strings
#define STB_C_LEX_USE_STDLIB \
  N  // use strtod,strtol for parsing #s; otherwise inaccurate hack
#define STB_C_LEX_DOLLAR_IDENTIFIER Y  // allow $ as an identifier character
#define STB_C_LEX_FLOAT_NO_DECIMAL \
  Y  // allow floats that have no decimal point if they have an exponent

#define STB_C_LEX_DEFINE_ALL_TOKEN_NAMES \
  Y  // if Y, all CLEX_ token names are defined, even if never returned
     // leaving it as N should help you catch config bugs

#define STB_C_LEX_DISCARD_PREPROCESSOR \
  Y  // discard C-preprocessor directives (e.g. after prepocess
     // still have #line, #pragma, etc)

#define STB_C_LEXER_DEFINITIONS  // This line prevents the header file from
                                 // replacing your definitions

#define STB_C_LEXER_IMPLEMENTATION

#include "stb_c_lexer.h"

static void print_token(stb_lexer* lexer) {
  switch (lexer->token) {
    case CLEX_id:
      printf("{{%s}}", lexer->string);
      break;
    case CLEX_eq:
      printf("==");
      break;
    case CLEX_noteq:
      printf("!=");
      break;
    case CLEX_lesseq:
      printf("<=");
      break;
    case CLEX_greatereq:
      printf(">=");
      break;
    case CLEX_andand:
      printf("&&");
      break;
    case CLEX_oror:
      printf("||");
      break;
    case CLEX_shl:
      printf("<<");
      break;
    case CLEX_shr:
      printf(">>");
      break;
    case CLEX_plusplus:
      printf("++");
      break;
    case CLEX_minusminus:
      printf("--");
      break;
    case CLEX_arrow:
      printf("->");
      break;
    case CLEX_andeq:
      printf("&=");
      break;
    case CLEX_oreq:
      printf("|=");
      break;
    case CLEX_xoreq:
      printf("^=");
      break;
    case CLEX_pluseq:
      printf("+=");
      break;
    case CLEX_minuseq:
      printf("-=");
      break;
    case CLEX_muleq:
      printf("*=");
      break;
    case CLEX_diveq:
      printf("/=");
      break;
    case CLEX_modeq:
      printf("%%=");
      break;
    case CLEX_shleq:
      printf("<<=");
      break;
    case CLEX_shreq:
      printf(">>=");
      break;
    case CLEX_eqarrow:
      printf("=>");
      break;
    case CLEX_dqstring:
      printf("\"%s\"", lexer->string);
      break;
    case CLEX_sqstring:
      printf("'\"%s\"'", lexer->string);
      break;
    case CLEX_charlit:
      printf("'%s'", lexer->string);
      break;
#if defined(STB__clex_int_as_double) && !defined(STB__CLEX_use_stdlib)
    case CLEX_intlit:
      printf("#%g", lexer->real_number);
      break;
#else
    case CLEX_intlit:
      printf("#%ld", lexer->int_number);
      break;
#endif
    case CLEX_floatlit:
      printf("%g", lexer->real_number);
      break;
    default:
      if (lexer->token >= 0 && lexer->token < 256)
        printf("_%c", (int)lexer->token);
      else {
        printf("<<<UNKNOWN TOKEN %ld >>>\n", lexer->token);
      }
      break;
  }
}

int main(int argc, char* argv[]) {
  const char* c_input =
      "#include <stdio.h>\n\nint main(int argc, char *argv[]) {\n  "
      "printf(\"Hello, World!\");\n  return 0;\n}\n\0";
  int len_input = strlen(c_input);
  printf("input:\n%s\n", c_input);
  printf("length:\n%d\n", len_input);
  stb_lexer lexer;
  stb_c_lexer_init(&lexer, c_input, c_input + len_input, (char*)malloc(0x10000),
                   0x10000);
  while (stb_c_lexer_get_token(&lexer)) {
    if (lexer.token == CLEX_parse_error) {
      printf("\n<<<PARSE ERROR>>>\n");
      break;
    }
    print_token(&lexer);
    printf("  ");
  }

  return 0;
}