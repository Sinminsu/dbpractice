#include <iostream>
#include <sqlite3.h>
using namespace std;

void base_chk(int n, const char* err, sqlite3* DB) {
  if (n) {
    cout << err << '\n' << sqlite3_errmsg(DB) << endl;
    exit(1);
  }
}

void chk(int n, const char* str, sqlite3* DB, char* errmsg) {
  if (n) {
    cout << str << '\n' << errmsg << endl;
    sqlite3_close(DB);
    exit(1);
  }
}

void chk(int n, const char* err, sqlite3* DB) {
  if (n) {
    cout << err << '\n' << sqlite3_errmsg(DB) << endl;
    sqlite3_close(DB);
    exit(1);
  }
}

int main() {
  sqlite3* db;
  char* err = NULL;
  //연결
  int rc = sqlite3_open("sj.db", &db);
  base_chk(rc, "데이터 베이스 열기에 실패했습니다", db);

  //테이블 생성
  const char* sql = "create table if not exists score("
    "id integer primary key autoincrement,"
    "name text not null, cpp integer  not null,"
    "network integer not null);";

  sqlite3_stmt* stmt;

  rc = sqlite3_exec(db, sql, NULL, 0, &err);
  chk(rc, "테이블 생성 실패", db, err);

  //성적 입력
  sql = "insert into score(name, cpp, network) values"
    "('오연주', 60, 35), ('이시우', 72, 60), ('박재원', 68, 73);";

  rc = sqlite3_exec(db, sql, NULL, 0, &err);
  chk(rc, "성적 입력 실패", db, err);

  //읽어오기
  sql = "select name, cpp, network from score;";
  rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  chk(rc, "성적 조회 실패", db);

  char sco[3][2];
  short i = 0;
  short ctotal = 0, ntotal = 0;

  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW && i < 3) {
    sco[i][0] = sqlite3_column_int(stmt, 1);
    sco[i][1] = sqlite3_column_int(stmt, 2);
    short total = sco[i][0] + sco[i][1];
    ctotal += sco[i][0];
    ntotal += sco[i][1];

    cout << "이름: " << sqlite3_column_text(stmt, 0) << "\t총점: " << total << "\t평균: " << total / 2.0 << endl;
    i++;
  }
  sqlite3_close(db);

  cout << "과목: c++" << "\t총점: " << ctotal << "\t평균: " << ctotal / 3.0 << endl;
  cout << "과목: network" << "\t총점: " << ntotal << "\t평균: " << ntotal / 3.0 << endl;
}