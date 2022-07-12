
#line 1 "dnslabeltext.rl"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "dnsname.hh"
#include "namespaces.hh"
#include "dnswriter.hh"
#include "misc.hh"

namespace {
void appendSplit(vector<string>& ret, string& segment, char c)
{
  if(segment.size()>254) {
    ret.push_back(segment);
    segment.clear();
  }
  segment.append(1, c);
}

}

vector<string> segmentDNSText(const string& input )
{
  // cerr<<"segmentDNSText("<<input<<")"<<endl; 

#line 30 "dnslabeltext.cc"
static const char _dnstext_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 2, 0, 1, 
	2, 4, 5
};

static const char _dnstext_key_offsets[] = {
	0, 0, 1, 3, 5, 7, 9, 11, 
	15
};

static const unsigned char _dnstext_trans_keys[] = {
	34u, 34u, 92u, 48u, 57u, 48u, 57u, 48u, 
	57u, 34u, 92u, 32u, 34u, 9u, 13u, 34u, 
	0
};

static const char _dnstext_single_lengths[] = {
	0, 1, 2, 0, 0, 0, 2, 2, 
	1
};

static const char _dnstext_range_lengths[] = {
	0, 0, 0, 1, 1, 1, 0, 1, 
	0
};

static const char _dnstext_index_offsets[] = {
	0, 0, 2, 5, 7, 9, 11, 14, 
	18
};

static const char _dnstext_trans_targs[] = {
	2, 0, 7, 3, 2, 4, 2, 5, 
	0, 6, 0, 7, 3, 2, 8, 2, 
	8, 0, 2, 0, 0
};

static const char _dnstext_trans_actions[] = {
	3, 0, 0, 0, 11, 7, 5, 7, 
	0, 7, 0, 9, 9, 16, 0, 13, 
	0, 0, 13, 0, 0
};

static const char _dnstext_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 1, 
	1
};

static const int dnstext_start = 1;
static const int dnstext_first_final = 7;
static const int dnstext_error = 0;

static const int dnstext_en_main = 1;


#line 30 "dnslabeltext.rl"

	(void)dnstext_error;  // silence warnings
	(void)dnstext_en_main;
        const char *p = input.c_str(), *pe = input.c_str() + input.length();
        const char* eof = pe;
        int cs;
        char val = 0;

        string segment;
        vector<string> ret;

        
#line 100 "dnslabeltext.cc"
	{
	cs = dnstext_start;
	}

#line 105 "dnslabeltext.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const unsigned char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _dnstext_trans_keys + _dnstext_key_offsets[cs];
	_trans = _dnstext_index_offsets[cs];

	_klen = _dnstext_single_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _dnstext_range_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _dnstext_trans_targs[_trans];

	if ( _dnstext_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _dnstext_actions + _dnstext_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 42 "dnslabeltext.rl"
	{ 
                        ret.push_back(segment);
                        segment.clear();
                }
	break;
	case 1:
#line 46 "dnslabeltext.rl"
	{ 
                        segment.clear();
                }
	break;
	case 2:
#line 50 "dnslabeltext.rl"
	{
                  char c = *p;
                  appendSplit(ret, segment, c);
                }
	break;
	case 3:
#line 54 "dnslabeltext.rl"
	{
                  char c = *p;
                  val *= 10;
                  val += c-'0';
                  
                }
	break;
	case 4:
#line 60 "dnslabeltext.rl"
	{
                  appendSplit(ret, segment, val);
                  val=0;
                }
	break;
	case 5:
#line 65 "dnslabeltext.rl"
	{
                  appendSplit(ret, segment, *(p));
                }
	break;
#line 220 "dnslabeltext.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _dnstext_actions + _dnstext_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
#line 42 "dnslabeltext.rl"
	{ 
                        ret.push_back(segment);
                        segment.clear();
                }
	break;
#line 243 "dnslabeltext.cc"
		}
	}
	}

	_out: {}
	}

#line 78 "dnslabeltext.rl"


        if ( cs < dnstext_first_final ) {
                throw runtime_error("Unable to parse DNS TXT '"+input+"'");
        }

        return ret;
};


DNSName::string_t segmentDNSNameRaw(const char* realinput, size_t inputlen)
{

#line 265 "dnslabeltext.cc"
static const char _dnsnameraw_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 2, 1, 5, 
	2, 4, 0, 2, 4, 5
};

static const char _dnsnameraw_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12
};

static const unsigned char _dnsnameraw_trans_keys[] = {
	46u, 92u, 48u, 57u, 48u, 57u, 48u, 57u, 
	46u, 92u, 46u, 92u, 46u, 92u, 0
};

static const char _dnsnameraw_single_lengths[] = {
	0, 2, 0, 0, 0, 2, 2, 2
};

static const char _dnsnameraw_range_lengths[] = {
	0, 0, 1, 1, 1, 0, 0, 0
};

static const char _dnsnameraw_index_offsets[] = {
	0, 0, 3, 5, 7, 9, 12, 15
};

static const char _dnsnameraw_trans_targs[] = {
	0, 2, 5, 3, 5, 4, 0, 7, 
	0, 6, 2, 5, 0, 2, 5, 6, 
	2, 5, 0
};

static const char _dnsnameraw_trans_actions[] = {
	0, 3, 13, 7, 5, 7, 0, 7, 
	0, 1, 0, 11, 0, 3, 13, 16, 
	9, 19, 0
};

static const char _dnsnameraw_eof_actions[] = {
	0, 0, 0, 0, 0, 1, 0, 16
};

static const int dnsnameraw_start = 1;
static const int dnsnameraw_first_final = 5;
static const int dnsnameraw_error = 0;

static const int dnsnameraw_en_main = 1;


#line 94 "dnslabeltext.rl"

	(void)dnsnameraw_error;  // silence warnings
	(void)dnsnameraw_en_main;

        DNSName::string_t ret;

        if(!*realinput || *realinput == '.') {
          ret.append(1, (char)0);
          return ret;
        }

        ret.reserve(inputlen+1);

        const char *p = realinput, *pe = realinput + inputlen;
        const char* eof = pe;
        int cs;
        char val = 0;
        char labellen=0;
        unsigned int lenpos=0;
        
#line 337 "dnslabeltext.cc"
	{
	cs = dnsnameraw_start;
	}

#line 342 "dnslabeltext.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const unsigned char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _dnsnameraw_trans_keys + _dnsnameraw_key_offsets[cs];
	_trans = _dnsnameraw_index_offsets[cs];

	_klen = _dnsnameraw_single_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _dnsnameraw_range_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _dnsnameraw_trans_targs[_trans];

	if ( _dnsnameraw_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _dnsnameraw_actions + _dnsnameraw_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 114 "dnslabeltext.rl"
	{ 
                        if (labellen < 0 || labellen > 63) {
                          throw runtime_error("Unable to parse DNS name '"+string(realinput)+"': invalid label length "+std::to_string(labellen));
                        }
                        ret[lenpos]=labellen;
                        labellen=0;
                }
	break;
	case 1:
#line 121 "dnslabeltext.rl"
	{ 
                        lenpos=ret.size();
                        ret.append(1, (char)0);
                        labellen=0;
                }
	break;
	case 2:
#line 127 "dnslabeltext.rl"
	{
                  char c = *p;
                  ret.append(1, c);
                  labellen++;
                }
	break;
	case 3:
#line 132 "dnslabeltext.rl"
	{
                  char c = *p;
                  val *= 10;
                  val += c-'0';
                }
	break;
	case 4:
#line 137 "dnslabeltext.rl"
	{
                  ret.append(1, val);
                  labellen++;
                  val=0;
                }
	break;
	case 5:
#line 143 "dnslabeltext.rl"
	{
                  ret.append(1, *(p));
                  labellen++;
                }
	break;
#line 464 "dnslabeltext.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _dnsnameraw_actions + _dnsnameraw_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
#line 114 "dnslabeltext.rl"
	{ 
                        if (labellen < 0 || labellen > 63) {
                          throw runtime_error("Unable to parse DNS name '"+string(realinput)+"': invalid label length "+std::to_string(labellen));
                        }
                        ret[lenpos]=labellen;
                        labellen=0;
                }
	break;
	case 4:
#line 137 "dnslabeltext.rl"
	{
                  ret.append(1, val);
                  labellen++;
                  val=0;
                }
	break;
#line 498 "dnslabeltext.cc"
		}
	}
	}

	_out: {}
	}

#line 164 "dnslabeltext.rl"


        if ( cs < dnsnameraw_first_final ) {
                throw runtime_error("Unable to parse DNS name '"+string(realinput)+"': cs="+std::to_string(cs));
        }
        ret.append(1, (char)0);
        return ret;
};

// Reads an RFC 1035 character string from 'in', puts the resulting bytes in 'out'.
// Returns the amount of bytes read from 'in'
size_t parseRFC1035CharString(const std::string &in, std::string &val) {

  val.clear();
  val.reserve(in.size());
  const char *p = in.c_str();
  const char *pe = p + in.size();
  int cs = 0;
  uint8_t escaped_octet = 0;
  // Keeps track of how many chars we read from the source string
  size_t counter=0;

/* This parses an RFC 1035 char-string.
 * It was created from the ABNF in draft-ietf-dnsop-svcb-https-02 with
 * https://github.com/zinid/abnfc and modified to put all the characters in the
 * right place.
 */

#line 535 "dnslabeltext.cc"
static const char _dns_text_to_string_actions[] = {
	0, 1, 0, 1, 2, 1, 3, 2, 
	0, 1
};

static const char _dns_text_to_string_key_offsets[] = {
	0, 0, 8, 15, 17, 19, 22, 24, 
	33, 41, 43, 45, 48, 50, 58
};

static const char _dns_text_to_string_trans_keys[] = {
	34, 92, 33, 39, 42, 58, 60, 126, 
	50, 33, 47, 48, 49, 58, 126, 48, 
	57, 48, 57, 53, 48, 52, 48, 53, 
	9, 34, 92, 32, 39, 42, 58, 60, 
	126, 9, 50, 32, 47, 48, 49, 58, 
	126, 48, 57, 48, 57, 53, 48, 52, 
	48, 53, 33, 92, 35, 39, 42, 58, 
	60, 126, 0
};

static const char _dns_text_to_string_single_lengths[] = {
	0, 2, 1, 0, 0, 1, 0, 3, 
	2, 0, 0, 1, 0, 2, 0
};

static const char _dns_text_to_string_range_lengths[] = {
	0, 3, 3, 1, 1, 1, 1, 3, 
	3, 1, 1, 1, 1, 3, 0
};

static const char _dns_text_to_string_index_offsets[] = {
	0, 0, 6, 11, 13, 15, 18, 20, 
	27, 33, 35, 37, 40, 42, 48
};

static const char _dns_text_to_string_indicies[] = {
	2, 3, 0, 0, 0, 1, 5, 0, 
	4, 0, 1, 6, 1, 7, 1, 8, 
	6, 1, 7, 1, 9, 10, 11, 9, 
	9, 9, 1, 9, 13, 9, 12, 9, 
	1, 14, 1, 15, 1, 16, 14, 1, 
	15, 1, 0, 3, 0, 0, 0, 1, 
	1, 0
};

static const char _dns_text_to_string_trans_targs[] = {
	13, 0, 7, 2, 3, 5, 4, 13, 
	6, 7, 14, 8, 9, 11, 10, 7, 
	12
};

static const char _dns_text_to_string_trans_actions[] = {
	3, 0, 5, 5, 1, 1, 1, 7, 
	1, 3, 5, 5, 1, 1, 1, 7, 
	1
};

static const int dns_text_to_string_start = 1;
static const int dns_text_to_string_first_final = 13;
static const int dns_text_to_string_error = 0;

static const int dns_text_to_string_en_main = 1;


#line 601 "dnslabeltext.cc"
	{
	cs = dns_text_to_string_start;
	}

#line 232 "dnslabeltext.rl"


  // silence warnings
  (void) dns_text_to_string_first_final;
  (void) dns_text_to_string_error;
  (void) dns_text_to_string_en_main;
  
#line 614 "dnslabeltext.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _dns_text_to_string_trans_keys + _dns_text_to_string_key_offsets[cs];
	_trans = _dns_text_to_string_index_offsets[cs];

	_klen = _dns_text_to_string_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _dns_text_to_string_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _dns_text_to_string_indicies[_trans];
	cs = _dns_text_to_string_trans_targs[_trans];

	if ( _dns_text_to_string_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _dns_text_to_string_actions + _dns_text_to_string_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 194 "dnslabeltext.rl"
	{
    escaped_octet *= 10;
    escaped_octet += (*p)-'0';
    counter++;
  }
	break;
	case 1:
#line 200 "dnslabeltext.rl"
	{
    val += escaped_octet;
    escaped_octet = 0;
  }
	break;
	case 2:
#line 205 "dnslabeltext.rl"
	{
    val += (*p);
    counter++;
  }
	break;
	case 3:
#line 210 "dnslabeltext.rl"
	{
    counter++;
  }
	break;
#line 716 "dnslabeltext.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 239 "dnslabeltext.rl"

  return counter;
}

size_t parseSVCBValueListFromParsedRFC1035CharString(const std::string &in, std::vector<std::string> &val) {
  val.clear();
  const char *p = in.c_str();
  const char *pe = p + in.size();
  int cs = 0;
  const char* eof = pe;
  // Keeps track of how many chars we read from the source string
  size_t counter=0;

  // Here we store the parsed value until we hit a comma or are done
  std::string tmp;


#line 747 "dnslabeltext.cc"
static const char _dns_text_to_value_list_actions[] = {
	0, 1, 0, 1, 2, 1, 3, 2, 
	2, 3, 2, 3, 1
};

static const char _dns_text_to_value_list_key_offsets[] = {
	0, 0, 2, 4, 6
};

static const unsigned char _dns_text_to_value_list_trans_keys[] = {
	44u, 92u, 44u, 92u, 44u, 92u, 44u, 92u, 
	0
};

static const char _dns_text_to_value_list_single_lengths[] = {
	0, 2, 2, 2, 2
};

static const char _dns_text_to_value_list_range_lengths[] = {
	0, 0, 0, 0, 0
};

static const char _dns_text_to_value_list_index_offsets[] = {
	0, 0, 3, 6, 9
};

static const char _dns_text_to_value_list_indicies[] = {
	1, 2, 0, 3, 3, 1, 1, 2, 
	0, 4, 2, 0, 0
};

static const char _dns_text_to_value_list_trans_targs[] = {
	4, 0, 2, 4, 1
};

static const char _dns_text_to_value_list_trans_actions[] = {
	1, 0, 5, 10, 7
};

static const char _dns_text_to_value_list_eof_actions[] = {
	0, 0, 0, 0, 3
};

static const int dns_text_to_value_list_start = 3;
static const int dns_text_to_value_list_first_final = 3;
static const int dns_text_to_value_list_error = 0;

static const int dns_text_to_value_list_en_main = 3;


#line 798 "dnslabeltext.cc"
	{
	cs = dns_text_to_value_list_start;
	}

#line 288 "dnslabeltext.rl"


  // silence warnings
  (void) dns_text_to_value_list_first_final;
  (void) dns_text_to_value_list_error;
  (void) dns_text_to_value_list_en_main;
  
#line 811 "dnslabeltext.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const unsigned char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _dns_text_to_value_list_trans_keys + _dns_text_to_value_list_key_offsets[cs];
	_trans = _dns_text_to_value_list_index_offsets[cs];

	_klen = _dns_text_to_value_list_single_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _dns_text_to_value_list_range_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _dns_text_to_value_list_indicies[_trans];
	cs = _dns_text_to_value_list_trans_targs[_trans];

	if ( _dns_text_to_value_list_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _dns_text_to_value_list_actions + _dns_text_to_value_list_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 259 "dnslabeltext.rl"
	{
    tmp += (*p);
    counter++;
  }
	break;
	case 1:
#line 264 "dnslabeltext.rl"
	{
    tmp += (*p);
  }
	break;
	case 2:
#line 268 "dnslabeltext.rl"
	{
    val.push_back(tmp);
    tmp.clear();
    counter++;
  }
	break;
	case 3:
#line 274 "dnslabeltext.rl"
	{
    counter++;
  }
	break;
#line 912 "dnslabeltext.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _dns_text_to_value_list_actions + _dns_text_to_value_list_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 2:
#line 268 "dnslabeltext.rl"
	{
    val.push_back(tmp);
    tmp.clear();
    counter++;
  }
	break;
#line 936 "dnslabeltext.cc"
		}
	}
	}

	_out: {}
	}

#line 295 "dnslabeltext.rl"

  if ( cs < dns_text_to_value_list_first_final ) {
          throw runtime_error("Unable to parse DNS SVCB value list '"+in+"'");
  }

  return counter;
}


#if 0
int main()
{
	//char blah[]="\"blah\" \"bleh\" \"bloeh\\\"bleh\" \"\\97enzo\"";
  char blah[]="\"v=spf1 ip4:67.106.74.128/25 ip4:63.138.42.224/28 ip4:65.204.46.224/27 \\013\\010ip4:66.104.217.176/28 \\013\\010ip4:209.48.147.0/27 ~all\"";
  //char blah[]="\"abc \\097\\098 def\"";
  printf("Input: '%s'\n", blah);
	vector<string> res=dnstext(blah);
  cerr<<res.size()<<" segments"<<endl;
  cerr<<res[0]<<endl;
}
#endif
