// CDEFGAB(ドレミファソラシド)  対応
// 音符の後ろの長さの数字       対応
// .  付点(1.5倍)               対応
// R  休符                      対応
// T  テンポ                    対応
// V  ボリューム                対応
// L  長さセット                対応
// O  オクターブ変化            対応
// <  1オクターブ下げる         対応
// >  1オクターブ上げる         対応
// P  パン                      未着手
// #  シャープ                  たぶん対応
// &  タイ                      未着手
// P  ポルタメント              未着手
//--------------------------------------------
const char* phoneticSymbols[] = {
  "a", "i", "M", "e", "o",                 // あいうえお  00-04
  "k a", "k' i", "k M", "k e", "k o",      // かきくけこ  05-09
  "s a", "S i", "s M", "s e", "s o",       // さしすえそ  10-14
  "t a", "tS i", "ts M", "t e", "t o",     // たちつてと  15-19
  "n a", "J i", "n M", "n e", "n o",       // なにぬねの  20-24
  "h a", "C i", "p\\ M", "h e", "h o",     // はひふへほ  25-29
  "m a", "m' i", "m M", "m e", "m o",      // まみむめも  30-34
  "j a","i", "j M","e","j o",              // やいゆえよ  35-39
  "4 a", "4' i", "4 M", "4 e", "4 o",      // らりるれろ  40-44
  "w a","w o","N\\","","",                 // わをん      45-49
  "g a", "g' i", "g M", "g e", "g o",      // がぎぐげご  50-54
  "dz a", "dZ i", "dz M", "dz e", "dz o",  // ざじずぜぞ  55-59
  "d a", "dZ i", "dz M", "d e", "d o",     // だじづでど  60-64
  "b a", "b' i", "b M", "b e", "b o",      // ばびぶべぼ  65-69
  "p a", "p' i", "p M", "p e", "p o"       // ぱぴぷぺぽ  70-74
};
//--------------------------------------------
// 歌詞
//int voice[] = {25,31,25,31,25,31,1,30,25,31,25,31,30,0};
int voice[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,
  0,1,2,3,4,5,6,7,8,9,
  0,0,0,0,0,
  0,1,2,3,4,
  5,6,7,8,9,
  10,11,12,13,14,
  50,50,50,50
};
//int voice[] = {6,47,9,47,5,47,9,47,6,47,9,47,5,47,9,47};
// メロディ
//char melody[] = "R1T60L8O6EC>G<CDGGGDED>G<CC4";
char melody[] = "o5cdefgab<cdefgab<co5v1cv3cv7cv15cv31cv63cv127v8cccccccccccrcrrcrrrcrrrrcrrrrrl1cl2cl4cl8cl16cl32cl1rt10crt20crt30crt40crt50crt40c16c8c4c2c1r2l2crc2rc2.rc.";
//char melody[] = "T80L4O5FF32AA32GG32CC16RCC32GG32AA32FF32";
//--------------------------------------------

char tempo,tempo_now;
char volume,volume_now;
char octave,octave_now;
char time,time_now;
int temp_int;
int melody_counter;
char old_key;
char time_table[] = {0,32,16,0,8,0,0,0,4,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};

void setup() {
  pinMode(13, OUTPUT);  // データエラーが発生したらArduinoのLEDが光ります
  digitalWrite(13, LOW);
  //  Set MIDI baud rate
  Serial.begin(31250);
  // waka
  tempo = 40;
  time = 4;
  volume = 5;
  octave = 4;
  delay(3500);
}

void loop() {
  boolean flag;
  tempo_now = tempo;
  time_now = time;
  volume_now = volume;
  octave_now = octave;
  // 歌詞転送
  int voice_len = sizeof(voice) / sizeof(voice[0]);
  send_voice(voice_len);
  int melody_len = sizeof(melody) / sizeof(melody[0]);
  // ＭＭＬ解析＆ＰＬＡＹ
  for (melody_counter = 0;melody_counter < melody_len - 1;melody_counter++) {
    flag = true;
    while (flag) {
      char c = melody[melody_counter];
      if ((c >= 'a') && (c <= 'z')) {  // 小文字を大文字に補正
        c -= 0x20;
      }
      if (((c >= 'A') && (c <= 'G')) || (c == 'R')) {  // 音符または休符
        int time_temp;
        if (c != 'R') {
          char table[] = {9,11,0,2,4,5,7};  // A,B,C,D,E,F,G
          old_key = table[c - 'A'] + octave_now * 12;
          if (melody[melody_counter + 1] == '#') {
            old_key++;
            melody_counter++;
          }
          Short_Message(0x90,old_key,volume_now);  // 鳴らす
        } else {
          Short_Message(0x80,old_key,volume_now);  // 止める
        }
        read_num();
        if (temp_int != -1) {
          if ((temp_int > 0) && (temp_int <= 32)) {
            melody_counter--;
            time_temp = time_table[temp_int];
          } else {
            digitalWrite(13, HIGH);
            while(true) {}
          }
        } else {
          time_temp = time_table[time_now];
        }
        if (melody[melody_counter + 1] == '.') {
          time_temp = time_temp * 15 / 10;
          melody_counter++;
        }
        delay(tempo_now * time_temp);  // 通常の音符長
        flag = false;
      } else {  // 音符以外
        switch (c) {
          case 'T':  // テンポのセット(5～100)
            read_num();
            if ((temp_int == -1) || (temp_int < 5) || (temp_int > 100)) {
              digitalWrite(13, HIGH);
              while(true) {}
            } 
            tempo_now = (char)temp_int;
            break;
          
          case 'V':  // ボリュームのセット(0～127)
            read_num();
            if ((temp_int == -1) || (temp_int < 0) || (temp_int > 127)) {
              digitalWrite(13, HIGH);
              while(true) {}
            } 
            volume_now = (char)temp_int;
            break;
          case 'O':  // オクターブのセット(0～10)
            read_num();
            if ((temp_int == -1) || (temp_int < 0) || (temp_int > 10)) {
              digitalWrite(13, HIGH);
              while(true) {}
            } 
            octave_now = (char)temp_int;
            break;
          case '>':  // オクターブ1下げる
            if (octave >= 0) {
              octave_now--;
              melody_counter++;
            } else {
              digitalWrite(13, HIGH);
              while(true) {}
            }
            break;
          case '<':  // オクターブ1上げる
            if (octave <= 10) {
              octave_now++;
              melody_counter++;
            } else {
              digitalWrite(13, HIGH);
              while(true) {}
            }
            break;
          case 'L':  // 音の長さのセット(1,2,4,8,16,32)
            read_num();
            if ((temp_int > 0) && (temp_int <= 32)) {
              time_now = temp_int;
            } else {
              digitalWrite(13, HIGH);
              while (true) {}
            }
            break;
        }
      }
    }
  }
  Short_Message(0x80,old_key,volume_now);  // 止める
  delay(2000);
}

void Short_Message(int cmd, int d1, int d2) {
  Serial.write(cmd);
  Serial.write(d1);
  Serial.write(d2);
}

// 歌詞転送
void send_voice(int len) {
  // hedaer
  Serial.write(0xF0);
  Serial.write(0x43);
  Serial.write(0x79);
  Serial.write(0x09);
  Serial.write(0x00);
  Serial.write(0x50);
  Serial.write(0x10);
  // 歌詞
  for (int i = 0;i < len;i++) {
    if(i != 0) Serial.write(0x2c);
    Serial.write(phoneticSymbols[voice[i]]);
  }
  // footer
  Serial.write(0x00);
  Serial.write(0xF7);
}

void read_num() {
  boolean flag = true;
  temp_int = 0;
  while (true) {
    if ((melody[melody_counter + 1] >= '0') && (melody[melody_counter + 1] <= '9')) {
      temp_int = temp_int * 10 + melody[++melody_counter] - '0';
      flag = false;
    } else {
      if (flag == true) temp_int = -1;
      else melody_counter++;
      break;
    }
  }
}

