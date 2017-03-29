#!/usr/bin/env python3

# component: info manager, passwd generator

# input 1: global password, should be remembered by users themself
# input 2: characteristics of sites,etc could be recorded by our manager
# input 3: password format request ( capital, numbers, etc), this is a special characteristics of sites

# principle: we may calculated password by ourself with out this application ( although it maybe difficult to calculate by hand)
#            that is to say, if we remember global password, characteristics to this domain, algorithm, we do not need to open our manager

# extension: password sharing

# problem: recover password; change global password; sync; some password should only numbers or six char; version compatible

# 關於密碼設計，global密碼可以考慮設置成密碼滿足一定條件就可以打開wallet，但是不完全輸入正確就無法繼續得到對應的密碼（即global密碼既參與保管密碼本又參與生成密碼，具有一定的迷惑性的惡趣味）
# 分類：線下/線上，日常/錢，可以告訴別人/不能告訴別人

# prototype: f(global_password, domain_name) = specific_password

import os
import hashlib
import pickle

class PasswdManager:
    VERSION = "0.0.0"
    def __init__(self):
        self._records = {}

    def _extract_feature(self, domain):
        """ extract characteristics of sites
        we just use sha256 of domain name as an example """
        return hashlib.sha256(domain.encode()).digest()

    def add_record(self, domain):
        """ record a new domain """
        if domain in self._records:
            print("already exist, overwrite")
        self._records[domain] = self._extract_feature(domain)

    def query(self, domain):
        return self._records[domain]

class PasswdDB:
    def __init__(self, dbname = "test.db"):
        if os.path.isfile(dbname):
            data = open(dbname, 'rb').read()
            print("db found", data)
        elif not os.path.exists(dbname):
            data = None
            print("no db, should create new one")
        else:
            raise RuntimeError("db {dbname} is not file".format(dbname=dbname))
        self._dbname = dbname
        self._db = {} if data is None else pickle.loads(data)
        print(self._db)
    def add(self, passwd_glob, passwd_manager):
        if passwd_glob in self._db:
            print("alread exists")
        self._db[passwd_glob] = passwd_manager
    def save(self):
        open(self._dbname, 'wb').write(pickle.dumps(self._db))
    def get(self, passwd_glob):
        return self._db[passwd_glob]

class PasswdGenerator:
    VERSION = "0.0.0"
    @staticmethod
    def gen(input_glob: bytes, input_info: bytes, input_fmt = None):
        print(input_glob, input_info)
        p = ''
        input1 = hashlib.sha256(input_glob).digest()[:-4] # 4*7=28 bytes
        input2 = input_info[:-4] # 4*7 bytes
        output = [b'0'] * 7
        for i in range(len(output)):
            output[i] = PasswdGenerator.conv(input1[4*i:4*i+4], input2[4*i:4*i+4])
            output[i] = output[i] % 95 + 32
            p += chr(output[i])
        print(output)
        return p

    @staticmethod
    def conv(a, b):
        return \
        (((a[0] ^ b[0]) << 6) & 0xc0) | \
        (((a[1] ^ b[1]) << 4) & 0x30) | \
        (((a[2] ^ b[2]) << 2) & 0x0c) | \
        (((a[3] ^ b[3]) << 0) & 0x03)


if __name__ == '__main__':
    ph = PasswdManager()
    ph.add_record("www.baidu.com")
    db = PasswdDB()
    db.add("testpw123", ph)
    db.save()

    ph_get = db.get("testpw123")
    passwd = PasswdGenerator.gen(b"testpw123", ph_get.query("www.baidu.com"))
    print(passwd)
