#include "LibUtil.h"
#include <fstream>
#include <strstream>
#include <windows.h>
#include <assert.h>
#include <mbstring.h>
#include <shlobj.h>
#include <wininet.h>

double GetPreciseTime() {
	LARGE_INTEGER freq, count;
	if (QueryPerformanceFrequency(&freq) && 
		QueryPerformanceCounter(&count)) {
		return (double)count.QuadPart/freq.QuadPart;
	} else {
		return (double)GetTickCount()/1000;
	}
}

void OpenURL(string url) {
	ShellExecute(NULL, NULL, url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

string GetSpecialFolderPath(int csidl) {
	char buf[MAX_PATH];
	SHGetSpecialFolderPath(NULL, buf, csidl, false);
	return string(buf);
}

string GetDesktopFolderPath() {
	return GetSpecialFolderPath(CSIDL_DESKTOP);
}


// 拡張子の登録
bool RegFileExt(string ext, string inst, string command, bool force) {
	HKEY key;
	DWORD disp;
	string cls=ext+"_auto_file";
	
	// 拡張子の登録
	if (ERROR_SUCCESS!=RegCreateKeyEx(
		HKEY_CLASSES_ROOT, (string(".")+ext).c_str(), NULL, "", 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disp)) {
		return false;
	}
	if (!force && disp==REG_OPENED_EXISTING_KEY) {
		RegCloseKey(key);
		return false;
	}
	RegSetValueEx(key, NULL, 0, REG_SZ, (BYTE*)cls.c_str(), cls.length());
	RegCloseKey(key);

	// 説明の登録
	if (ERROR_SUCCESS!=RegCreateKeyEx(
		HKEY_CLASSES_ROOT, cls.c_str(), NULL, "", 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disp)) {
		return false;
	}
	RegSetValueEx(key, NULL, 0, REG_SZ, (BYTE*)inst.c_str(), inst.length());
	RegCloseKey(key);

	// コマンドの登録
	if (ERROR_SUCCESS!=RegCreateKeyEx(
		HKEY_CLASSES_ROOT, (cls+"\\shell\\open\\command").c_str(), NULL, "", 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disp)) {
		return false;
	}
	string cmd=string("\"")+command+"\" \"%1\"";
	RegSetValueEx(key, NULL, 0, REG_SZ, (BYTE*)cmd.c_str(), cmd.length());
	RegCloseKey(key);

	// アイコンの登録
	if (ERROR_SUCCESS!=RegCreateKeyEx(
		HKEY_CLASSES_ROOT, (cls+"\\DefaultIcon").c_str(), NULL, "", 
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disp)) {
		return false;
	}
	string icon=command+",0";
	RegSetValueEx(key, NULL, 0, REG_SZ, (BYTE*)icon.c_str(), icon.length());
	RegCloseKey(key);

	return true;
}


string TrimLeft(string s, string pattern) {
	int pl=pattern.length();
	while (StartsWith(s, pattern)) s=s.substr(pl);
	return s;
}

string TrimRight(string s, string pattern) {
	int sl=s.length(), pl=pattern.length();
	while (EndsWith(s, pattern)) s=s.substr(sl-pl);
	return s;
}

string Trim(string s, string pattern) {
	s=TrimLeft(s, pattern);
	s=TrimRight(s, pattern);
	return s;
}

int Find(string s, string pattern) {
	for (int i=0, n=s.length(), pl=pattern.length(); i<n; i++) {
		if (s.compare(i, pl, pattern)==0) return i;
		if (IsLeadByte(s[i])) i++;
	}
	return string::npos;
}

int RFind(string s, string pattern) {
	for (int i=s.length()-1, pl=pattern.length(); i>=0; i--) {
		if (s.compare(i, pl, pattern)==0 && 
			!IsSecondByte(s, i)) return i;
	}
	return string::npos;
}

string GetToken(string s, int index, string separator) {
	int sl=separator.length();
	for (int i=0; i<index; i++) {
		int pos=Find(s, separator);
		if (pos==string::npos) return "";
		s=s.substr(pos+sl);
	}
	return s.substr(0, Find(s, separator));
}

int GetTokenCount(string s, string separator) {
	int sl=separator.length(), i;
	for (i=1; ; i++) {
		int pos=Find(s, separator);
		if (pos==string::npos) break;
		s=s.substr(pos+sl);
	}
	return i;
}

static string ToHanSub(string s, const char* sa, const char* sz, char a) {
	unsigned int
		ca=(unsigned int)((unsigned char)sa[0])<<8|(unsigned char)sa[1],
		cz=(unsigned int)((unsigned char)sz[0])<<8|(unsigned char)sz[1];
	string t;
	while (!s.empty()) {
		if (IsLeadByte(s[0]) && s.length()>=2) {
			unsigned int 
				c=(unsigned int)((unsigned char)s[0])<<8|(unsigned char)s[1];
			if (ca<=c && c<=cz) {
				t+=(char)(a+(c-ca));
			} else {
				t+=s[0];
				t+=s[1];
			}
			s=s.substr(2);
		} else {
			t+=s[0];
			s=s.substr(1);
		}
	}
	return t;	
}

string ToHan123(string s) {
	return ToHanSub(s, "０", "９", '0');
}

string ToHanABC(string s) {
	string t=s;
	t=ToHanSub(t, "ａ", "ｚ", 'a');
	t=ToHanSub(t, "Ａ", "Ｚ", 'A');
	return t;
}

string Replace(string s, string from, string to) {
	if (Find(s, from)==string::npos) return s;
	string t;
	int fl=from.length();
	while (!s.empty()) {
		if (StartsWith(s, from)) {
			t+=to;
			s=s.substr(fl);
		} else
		if (IsLeadByte(s[0])) {
			t+=s[0];
			t+=s[1];
			s=s.substr(2);
		} else {
			t+=s[0];
			s=s.substr(1);
		}
	}
	return t;
}

string Replace2(string s, int n, string fromto[][2]) {
	int i;
	for (i=0; i<n; i++) {
		if (Find(s, fromto[i][0])!=string::npos) break;
	}
	if (i==n) return s;
	string t;
	while (!s.empty()) {
		for (i=0; i<n; i++) {
			if (StartsWith(s, fromto[i][0])) {
				t+=fromto[i][1];
				s=s.substr(fromto[i][0].length());
				break;
			}
		}
		if (i==n) {
			if (IsLeadByte(s[0])) {
				t+=s[0];
				t+=s[1];
				s=s.substr(2);
			} else {
				t+=s[0];
				s=s.substr(1);
			}
		}
	}
	return t;
}


float ToFloat(string s) {
	istrstream iss(s.c_str());
	float f=0;
	iss>>f;
	return f;
}

int ToInt(string s) {
	istrstream iss(s.c_str());
	int i=0;
	iss>>i;
	return i;
}




//==============================================================
// ファイル関連


// 実行ファイルのパスを返す（末尾に\が付く）
string GetExePath() {
	char buf[MAX_PATH+1];
	GetModuleFileName(NULL, buf, MAX_PATH);
	return ExtractFilePath(buf);
}


// 「パス＋ファイル名」からパス部分を抜き出す
string ExtractFilePath(string file) {
	unsigned long pos=RFind(file, "\\");
	if (pos==string::npos) return "";
	return file.substr(0, pos+1);
}


// 「パス＋ファイル名」からファイル名部分を抜き出す
string ExtractFileName(string file) {
	unsigned long pos=RFind(file, "\\");
	if (pos==string::npos) return file;
	return file.substr(pos+1);
}

// ファイルが存在するならtrueを返す
bool FileExists(string file) {
	WIN32_FIND_DATA fd;
	return FindFirstFile(file.c_str(), &fd)!=INVALID_HANDLE_VALUE;
}

// Web上のファイルを取得する
// urlはURLの文字列，dataは取得したデータ
// 戻り値はデータのバイト数
int GetWebFile(string url, char** data, string app_name) {
	HINTERNET inet, file;
	DWORD data_size=0, read_size;
	char* data_buf=NULL;

	// Webに接続する
	inet=InternetOpen(
		app_name.c_str(), INTERNET_OPEN_TYPE_PRECONFIG,
		NULL, NULL, 0);
	if (inet) {

		// URLで指定されたファイルを開く
		file=InternetOpenUrl(
			inet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
		if (file) {

			// 読み込み可能なファイルのサイズを調べる
			if (InternetQueryDataAvailable(
				file, &read_size, 0, 0)) {

				// ファイルを読み込む
				do {
					
					// 読み込み用のバッファを拡張する
					data_buf=(char*)realloc(data_buf, data_size+read_size+1);

					// ファイルの一部を読み込む
					InternetReadFile(
						file, data_buf+data_size, read_size, &read_size);

					// 読み込んだサイズを加算する
					data_size+=read_size;

				} while (read_size>0);
				data_buf[data_size]='\0';

			}

			// ファイルを閉じる
			InternetCloseHandle(file);
		}
		
		// 接続を閉じる
		InternetCloseHandle(inet);
	}

	*data=data_buf;
	return (int)data_size;
}


//==============================================================
// 文字列関連

// 大文字への変換
string ToUpper(string s) {
	string out=s;
	for (int i=0, n=out.size(); i<n; i++) {
		if (IsLeadByte(out[i])) { i++; continue; }
		out[i]=toupper(out[i]);
	}
	return out;
}


// 小文字への変換
string ToLower(string s) {
	string out=s;
	for (int i=0, n=out.size(); i<n; i++) {
		if (IsLeadByte(out[i])) { i++; continue; }
		out[i]=tolower(out[i]);
	}
	return out;
}

// 文字列の先頭が指定した文字列かどうかを調べる
bool StartsWith(string s, string pattern) {
	return s.compare(0, pattern.length(), pattern)==0;
}


// 文字列の末尾が指定した文字列かどうかを調べる
bool EndsWith(string s, string pattern) {
	int len=pattern.length();
	int pos=s.length()-len;
	if (pos<0 || s.compare(pos, len, pattern)!=0) return false;
	return !IsSecondByte(s, pos);
}


// 文字列の指定した位置が，
// 2バイト文字の2文字目かどうかを調べる
bool IsSecondByte(string s, int pos) {
	if ((unsigned long)pos>=s.length()) return false;
	int i;
	for (i=0; i<pos; i++) if (IsLeadByte(s[i])) i++;
	assert(i>=pos);
	return i>pos;
}


// 整数を文字列に変換する
string ToStr(int i) {
	char s[20];
	_itoa_s(i, s, sizeof(s), 10);
	return string(s);
}


// 文字列の中のエスケープシーケンスを変換する：
// \nは改行に，\tはタブに変換。
// その他の「\文字」は「文字」をそのまま出力する。
string ConvertEscapes(string s) {
	string ret;
	int i, n;
	for (i=0, n=s.length(); i<n; i++) {
		if (IsLeadByte(s[i]) && i+1<n) {
			ret+=s[i]; ret+=s[i+1];
			i++;
		} else
		if (s[i]=='\\' && i+1<n) {
			switch (s[i+1]) {
				case 'n': ret+='\n'; break;
				case 't': ret+='\t'; break;
				case '\n': break;
				default: ret+=s[i+1]; break;
			}
			i++;
		} else {
			ret+=s[i];
		}
	}
	return ret;
}


// 文字列の左側にある空白，タブ，改行を除去する
string TrimLeft(string s) {
	int i, n;
	for (i=0, n=s.length(); i<n ; i++)
		if (s[i]!=' ' && s[i]!='\t' && 
			s[i]!='\n' && s[i]!='\r') break;
	return s.substr(i, n-i);
}


// 文字列の右側にある空白，タブ，改行を除去する
string TrimRight(string s) {
	int i;
	for (i=s.length()-1; i>=0; i--)
		if (s[i]!=' ' && s[i]!='\t' &&
			s[i]!='\n' && s[i]!='\r') break;
	return s.substr(0, i+1);
}


// 文字列の両側にある空白，タブ，改行を除去する
string Trim(string s) {
	return TrimRight(TrimLeft(s));
}




//==============================================================
// 文字判定用のサブルーチン


// cがアルファベットならば真を返す
bool IsAlphabet(char c) {
	return 'a'<=c && c<='z' || 'A'<=c && c<='Z';
}


// cが数字ならば真を返す
bool IsDigit(char c) {
	return '0'<=c && c<='9';
}


// cが2バイト文字の1文字目ならば真を返す
bool IsLeadByte(char c) {
	int i=(unsigned char)c;
	return (0x81<=i && i<=0x9f) || (0xe0<=i && i<=0xfc);
}




//==============================================================
// CStringsクラス：複数行の文字列を扱う


// コンストラクタ
CStrings::CStrings() {
	Strings.clear();
}


// 末尾に文字列を追加する
void CStrings::Add(string s) {
	Strings.push_back(s);
}


// 全ての文字列を消去する
void CStrings::Clear() {
	Strings.clear();
}


// コピーする
void CStrings::Assign(CStrings* source) {
	Strings.assign(source->Strings.begin(), source->Strings.end());
}


// 行数を返す
int CStrings::GetCount() {
	return Strings.size();
}


// 指定した行を返す
string CStrings::GetString(int index) {
	if (index<0 || (int)Strings.size()<=index) return "";
	return Strings[index];
}


// 指定した行に文字列を設定する
void CStrings::SetString(int index, string str) {
	if (index<0 || (int)Strings.size()<=index) return;
	Strings[index]=str;
}


// 複数行の文字列を改行をはさんで連結し，単一の文字列として返す
string CStrings::GetText() {
	string s;
	int i, n;
	for (i=0, n=Strings.size(); i<n; i++) {
		s+=Strings[i];
		s+='\n';
	}
	return s;
}


// 複数行の文字列を改行で切り，複数の文字列にする
void CStrings::SetText(string text) {
	Clear();
	int pos;
	while ((pos=Find(text, "\n"))!=string::npos) {
		if (pos<1 || text[pos-1]!='\r') {
			Strings.push_back(text.substr(0, pos));
		} else {
			Strings.push_back(text.substr(0, pos-1));
		}
		text=text.substr(pos+1);
	}
	Strings.push_back(text);
}

// ファイルから複数行の文字列を読み込む
bool CStrings::LoadFromFile(string file_name) {
	ifstream fin;
	static const int BUFSIZE=1024;
	static char buf[BUFSIZE];
	Clear();
	fin.open(file_name.c_str());
	if (fin.fail()) return false;
	for (fin.getline(buf, BUFSIZE); !fin.fail(); fin.getline(buf, BUFSIZE))
		Strings.push_back(buf);
	fin.close();
	return true;
}

// ファイルに複数行の文字列を書き出す
bool CStrings::SaveToFile(string file_name) {
	ofstream fout;
	fout.open(file_name.c_str());
	if (fout.fail()) return false;
	fout<<GetText();
	fout.close();
	return true;
}

// Web上のファイルから複数行の文字列を読み込む
bool CStrings::LoadFromWeb(string url, string app_name) {
	char* buffer;
	if (!GetWebFile(url, &buffer, app_name)) return false;
	SetText(ToSJIS(buffer));
	free(buffer);
	return true;
}


//==============================================================
static const char* copyright=
	"Copyright(C) 2004 ひぐぺん工房（松浦健一郎／司ゆき）";


//==============================================================
// CStringsクラスの中で，
// 文字列を「名前=値」形式として解釈する関数群

// 指定した行の名前の部分を返す
string CStrings::GetName(int index) {
	if (index<0 || (int)Strings.size()<=index) return "";
	string s=Strings[index];
	int pos=Find(s, "=");
	if (pos<0) return "";
	return s.substr(0, pos);
}

// 指定した行の値の部分を返す
string CStrings::GetValue(int index) {
	if (index<0 || (int)Strings.size()<=index) return "";
	string s=Strings[index];
	int pos=Find(s, "=");
	if (pos<0) return "";
	return s.substr(pos+1, s.length()-pos-1);
}

// 指定した名前を持つ最初の行番号を返す
int CStrings::FindName(string name) {
	int i, n;
	for (i=0, n=Strings.size(); i<n; i++) {
		if (GetName(i)==name) return i;
	}
	return -1;
}

// 指定した名前を持つ最初の行を探し，値を返す：
// 名前が未登録ならば空文字列を返す
string CStrings::GetValue(string name) {
	int i=FindName(name);
	if (i<0) return "";
	return GetValue(i);
}

// 指定した名前を持つ最初の行を探し，文字列値を設定する
void CStrings::SetValue(string name, string value) {
	int i=FindName(name);
	if (i>=0) Strings[i]=name+"="+value;
	else Strings.push_back(name+"="+value);
}

// 指定した名前を持つ最初の行を探し，整数値を設定する
void CStrings::SetValue(string name, int value) {
	char buf[20];
	_itoa_s(value, buf, sizeof(buf), 10);
	SetValue(name, buf);
}


string Encode(string str) {
	string s=str;
	for (int i=0, n=s.length(), rnd=0xaa; i<n; i++) {
		s[i]=s[i]^(char)rnd;
		rnd=214013*rnd+2531011;
	}
	return ToBase64(s);
}

string Decode(string str) {
	string s=FromBase64(str);
	for (int i=0, n=s.length(), rnd=0xaa; i<n; i++) {
		s[i]=s[i]^(char)rnd;
		rnd=214013*rnd+2531011;
	}
	return s;
}


//==============================================================
// 文字コード関連


// 文字列のコードをJISからシフトJISに変換する：
// 入力文字列をJISからシフトJISに変換した文字列を返す。
string JISToSJIS(string s) {
	string ret;
	unsigned char c0, c1;
	bool kanji=false;

	for (int i=0, n=(int)s.length(); i<n; i++) {
	
		// エスケープシーケンスの判別
		if (s[i]==0x1b) {
			i++;
			if (i>=n) break;
			c0=s[i++];
			if (c0==0x24) kanji=true; else
			if (c0==0x28) kanji=false;
			continue;
		}

		// 漢字
		if (kanji) {
			c0=s[i++];
			if (i>=n) break;
			c1=s[i];
			ret+=(char)(((c0+1)>>1)+(c0<0x5f ? 0x70 : 0xb0));
			ret+=(char)(c1+(c0%2 ? (c1>0x5f ? 0x20 : 0x1f) : 0x7e));
		} else

		// ASCII文字
		{
			ret+=s[i];
		}
	}
	return ret;
}


// 文字列のコードをEUCからシフトJISに変換する：
// 入力文字列をEUCからシフトJISに変換した文字列を返す。
string EUCToSJIS(string s) {
	string ret;
	int i;
	unsigned char c0, c1;
	for (i=0; i<(int)s.length(); i++) {

		// 1バイト文字はそのままコピーする
		c0=s[i];
		if (c0<=0xa0 || c0==0xff || 
			(unsigned long)i==s.length()-1) {
			ret+=c0;
			continue;
		} 
		
		// 2バイト文字の2バイト目の変換
		c1=s[i+1];
		if (c0%2) {
			c1-=0x61;
			if (c1>0x7e) c1++;
		} else {
			c1-=0x02;
		}

		// 2バイト文字の1バイト目の変換
		if (c0>=0xdf) {
			c0=(c0+1)/2+0x70;
		} else {
			c0=(c0+1)/2+0x30;
		}
		ret+=c0;
		ret+=c1;
		i++;
	}
	return ret;
}


// 文字列のコードをシフトJISに変換する：
// 入力文字列の文字コードを判別して，EUCの場合にはシフトJISに変換する。
// 元々シフトJISの場合には何もしない。
string ToSJIS(string s) {
	int i;
	unsigned char c;
	for (i=0; i<(int)s.length(); i++) {

		// シフトJISにしかない文字を発見したら，
		// 変換せずに戻る
		c=s[i];
		if (0x81<=c && c<=0x9f) return s;
	}
	
	// シフトJISにしかない文字が発見できなかったので，
	// EUCだと想定して変換する
	return EUCToSJIS(s);
}


// BASE64形式の文字列をデコードする
string FromBase64(string s) {
	string ret;
	unsigned char c[4];
	for (int i=0, n=s.length(); i<n; i+=4) {
		int j;
		
		// 'A'～'Z'，'a'～'z'，'0'～'9'，'+'，'/’を，
		// 0～63の数値に変換する。
		for (j=0; j<4 && i+j<n; j++) {
			c[j]=s[i+j];
			if ('A'<=c[j] && c[j]<='Z') c[j]-='A'; else
			if ('a'<=c[j] && c[j]<='z') c[j]-='a'-26; else
			if ('0'<=c[j] && c[j]<='9') c[j]-='0'-52; else
			if (c[j]=='+') c[j]=62; else
			if (c[j]=='/') c[j]=63; else break;
		}
		
		// 6ビット値4個を，8ビット値3個に変換する
		if (j>1) ret+=(c[0]<<2 | c[1]>>4);
		if (j>2) ret+=(c[1]<<4 | c[2]>>2);
		if (j>3) ret+=(c[2]<<6 | c[3]);
	}
	return ret;
}


// 文字列をBASE64形式にエンコードする
string ToBase64(string s) {
	string ret;
	unsigned char c[3], d[4];
	for (int i=0, n=s.length(); i<n; i+=3) {
		int j;

		// 3文字を取り出す
		for (j=0; j<3; j++) c[j]=i+j<n?s[i+j]:0;
		
		// 8ビット値3個を，6ビット値4個に変換する
		d[0]=(c[0]>>2)&63;
		d[1]=(c[0]<<4|c[1]>>4)&63;
		d[2]=(c[1]<<2|c[2]>>6)&63;
		d[3]=c[2]&63;

		// 0～63の数値を，
		// 'A'～'Z'，'a'～'z'，'0'～'9'，'+'，'/’に変換する
		for (j=0; j<4; j++) {
			if (d[j]<=25) d[j]+='A'; else
			if (26<=d[j] && d[j]<=51) d[j]+='a'-26; else
			if (52<=d[j] && d[j]<=61) d[j]+='0'-52; else
			if (d[j]==62) d[j]='+'; else
			if (d[j]==63) d[j]='/'; else d[j]='A';
		}

		// 結果を文字列に追加する：
		// 文字数が4の倍数になるように，
		// 必要に応じて末尾に'='を加える
		ret+=d[0];
		ret+=d[1];
		ret+=i+1<n?d[2]:'=';
		ret+=i+2<n?d[3]:'=';
	}
	return ret;
}



