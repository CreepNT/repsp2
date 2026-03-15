
#include <sysmem/sysclib.h>

/* ctype.h implementation */
#define NUM_ASCII_CHARS 128
#define IS_ASCII(ch) ((0 <= (ch)) && ((ch) < NUM_ASCII_CHARS))

static const unsigned char ctype_table[NUM_ASCII_CHARS] = {
	/* 0x00~0x08: control codes */
	CTYPE_CONTROL, CTYPE_CONTROL, CTYPE_CONTROL,
	CTYPE_CONTROL, CTYPE_CONTROL, CTYPE_CONTROL,
	CTYPE_CONTROL, CTYPE_CONTROL, CTYPE_CONTROL,

	/* 0x09~0x0D: tab + whitespaces */
	CTYPE_SPACE, // 0x09: \t
	CTYPE_SPACE, // 0x0A: \n
	CTYPE_SPACE, // 0x0B: \v
	CTYPE_SPACE, // 0x0C: \f
	CTYPE_SPACE, // 0x0D: \r

	/* 0x0E~0x1F: control codes */
	CTYPE_CONTROL, CTYPE_CONTROL, CTYPE_CONTROL,
	CTYPE_CONTROL, CTYPE_CONTROL, CTYPE_CONTROL,
	CTYPE_CONTROL, CTYPE_CONTROL, CTYPE_CONTROL,
	CTYPE_CONTROL, CTYPE_CONTROL, CTYPE_CONTROL,
	CTYPE_CONTROL, CTYPE_CONTROL, CTYPE_CONTROL,
	CTYPE_CONTROL, CTYPE_CONTROL, CTYPE_CONTROL,

	/* 0x20: space */
	(CTYPE_PUNCT | CTYPE_SPACE), // bug(?): space is also punctuation?

	/* 0x21~0x2F: punctuation */
	CTYPE_PUNCT, // 0x21: !
	CTYPE_PUNCT, // 0x22: "
	CTYPE_PUNCT, // 0x23: #
	CTYPE_PUNCT, // 0x24: $
	CTYPE_PUNCT, // 0x25: %
	CTYPE_PUNCT, // 0x26: &
	CTYPE_PUNCT, // 0x27: '
	CTYPE_PUNCT, // 0x28: (
	CTYPE_PUNCT, // 0x29: )
	CTYPE_PUNCT, // 0x2A: *
	CTYPE_PUNCT, // 0x2B: +
	CTYPE_PUNCT, // 0x2C: ,
	CTYPE_PUNCT, // 0x2D: -
	CTYPE_PUNCT, // 0x2E: .
	CTYPE_PUNCT, // 0x2F: /

	/* 0x30~0x39: digits */
	CTYPE_DIGIT, // 0x30: 0
	CTYPE_DIGIT, // 0x31: 1
	CTYPE_DIGIT, // 0x32: 2
	CTYPE_DIGIT, // 0x33: 3
	CTYPE_DIGIT, // 0x34: 4
	CTYPE_DIGIT, // 0x35: 5
	CTYPE_DIGIT, // 0x36: 6
	CTYPE_DIGIT, // 0x37: 7
	CTYPE_DIGIT, // 0x38: 8
	CTYPE_DIGIT, // 0x39: 9

	/* 0x3A~0x40: punctuation */
	CTYPE_PUNCT, // 0x3A: :
	CTYPE_PUNCT, // 0x3B: ;
	CTYPE_PUNCT, // 0x3C: <
	CTYPE_PUNCT, // 0x3D: =
	CTYPE_PUNCT, // 0x3E: >
	CTYPE_PUNCT, // 0x3F: ?
	CTYPE_PUNCT, // 0x40: @

	/* 0x41~0x5A: uppercase letters */
	(CTYPE_UPPER | CTYPE_HEXDIGIT), // 0x41: A
	(CTYPE_UPPER | CTYPE_HEXDIGIT), // 0x42: B
	(CTYPE_UPPER | CTYPE_HEXDIGIT), // 0x43: C
	(CTYPE_UPPER | CTYPE_HEXDIGIT), // 0x44: D
	(CTYPE_UPPER | CTYPE_HEXDIGIT), // 0x45: E
	(CTYPE_UPPER | CTYPE_HEXDIGIT), // 0x46: F
	CTYPE_UPPER, // 0x47: G
	CTYPE_UPPER, // 0x48: H
	CTYPE_UPPER, // 0x49: I
	CTYPE_UPPER, // 0x4A: J
	CTYPE_UPPER, // 0x4B: K
	CTYPE_UPPER, // 0x4C: L
	CTYPE_UPPER, // 0x4D: M
	CTYPE_UPPER, // 0x4E: N
	CTYPE_UPPER, // 0x4F: O
	CTYPE_UPPER, // 0x50: P
	CTYPE_UPPER, // 0x51: Q
	CTYPE_UPPER, // 0x52: R
	CTYPE_UPPER, // 0x53: S
	CTYPE_UPPER, // 0x54: T
	CTYPE_UPPER, // 0x55: U
	CTYPE_UPPER, // 0x56: V
	CTYPE_UPPER, // 0x57: W
	CTYPE_UPPER, // 0x58: X
	CTYPE_UPPER, // 0x59: Y
	CTYPE_UPPER, // 0x5A: Z

	/* 0x5B~0x60: punctuation */
	CTYPE_PUNCT, // 0x5B: [
	CTYPE_PUNCT, // 0x5C: \ (backslash)
	CTYPE_PUNCT, // 0x5D: ]
	CTYPE_PUNCT, // 0x5E: ^
	CTYPE_PUNCT, // 0x5F: _
	CTYPE_PUNCT, // 0x60: `

	/* 0x61~0x7A: lowercase letters */
	(CTYPE_LOWER | CTYPE_HEXDIGIT), // 0x61: a
	(CTYPE_LOWER | CTYPE_HEXDIGIT), // 0x62: b
	(CTYPE_LOWER | CTYPE_HEXDIGIT), // 0x63: c
	(CTYPE_LOWER | CTYPE_HEXDIGIT), // 0x64: d
	(CTYPE_LOWER | CTYPE_HEXDIGIT), // 0x65: e
	(CTYPE_LOWER | CTYPE_HEXDIGIT), // 0x66: f
	CTYPE_LOWER, // 0x67: g
	CTYPE_LOWER, // 0x68: h
	CTYPE_LOWER, // 0x69: i
	CTYPE_LOWER, // 0x6A: j
	CTYPE_LOWER, // 0x6B: k
	CTYPE_LOWER, // 0x6C: l
	CTYPE_LOWER, // 0x6D: m
	CTYPE_LOWER, // 0x6E: n
	CTYPE_LOWER, // 0x6F: o
	CTYPE_LOWER, // 0x70: p
	CTYPE_LOWER, // 0x71: q
	CTYPE_LOWER, // 0x72: r
	CTYPE_LOWER, // 0x73: s
	CTYPE_LOWER, // 0x74: t
	CTYPE_LOWER, // 0x75: u
	CTYPE_LOWER, // 0x76: v
	CTYPE_LOWER, // 0x77: w
	CTYPE_LOWER, // 0x78: x
	CTYPE_LOWER, // 0x79: y
	CTYPE_LOWER, // 0x7A: z

	/* 0x7B~0x7E: punctuation */
	CTYPE_PUNCT, // 0x7B: {
	CTYPE_PUNCT, // 0x7C: |
	CTYPE_PUNCT, // 0x7D: }
	CTYPE_PUNCT, // 0x7E: ~

	/* 0x7F: control code (DEL) */
	CTYPE_CONTROL
};

int tolower(int ch) {
	if (IS_ASCII(ch) && (ctype_table[ch] & CTYPE_UPPER) != 0)
		return ch + 0x20; // 'a' - 'A' == 0x20

	return ch;
}

int toupper(int ch) {
	if (IS_ASCII(ch) && (ctype_table[ch] & CTYPE_LOWER) != 0)
		return ch - 0x20; // 'a' - 'A' == 0x20

	return ch;
}

unsigned look_ctype_table(int ch) {
	if (IS_ASCII(ch))
		return ctype_table[ch];

	return 0;
}
