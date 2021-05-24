import itertools
#key = 'Rgl6Pq8bdRxyTsVlb'
#['R']
#['b', 'c', 'd', 'e', 'f', 'g', 'i', 'n', 'r', 's', 'x', 'z']
#['e', 'f', 'h', 'i', 'l', 'm', 'n', 'o', 's', 'u', 'y']
#['3', '4', '5', '6', '7', '8']
#['D', 'E', 'O', 'P', 'Q', 'R', 'S', 'T']
#['q']
#['6', '8', '9']
#['b']
#['a', 'd', 'e', 'f', 'g', 'j', 'm', 'q']
#['G', 'M', 'O', 'P', 'Q', 'R', 'S', 'V', 'W']
#['x']
#['f', 'l', 'p', 'w', 'x', 'y', 'z']
#['A', 'K', 'P', 'Q', 'S', 'T', 'U', 'W', 'Z']
#['s']
#['V']
#['l']
#['b']
#guess_key = 'Rbe3Dq6baGxfAsVlb'
#cipher = b'\x1b\tL[?\x15]\x10\nr\x0c\x1c&\x1e?\x02\r>\x08\x0bO|QYB27\n\x175\x1ev\x0f\x0b"\x0f\tDp\x18KB\x05r\x0b\x009\x1e3\x18\x10;\x04\rZp\x02L\x10\x013\x15Y7\x1a&\x04\x07 G\x05Xp\x06P\x0b\x07:X\r<\x16v\x1c\x0e3\x0e\x02B5\tLB\r!X\x1a;\x1e4\x05\x0c7\x03LA9\x05PB\x05r\n\x18:\x179\x01B=\x15LF#\x14M\x06\x0b \x19\x170\x1c;L\x11&\x15\tW=QW\x04D6\x19\r5S~\x18\n7GN]5\x08K\x16\x167\x19\x14vZv\x03\x04r\x13\x04Sp\x02Y\x0f\x01r\x14\x1c:\x14"\x04Nr\x13\x03\x167\x14V\x07\x163\x0c\x1ct\x07>\tB1\x0e\x1c^5\x03L\x07\x1c&TY!\x00?\x02\x05r\x13\x04Sp3W\r\x087\x19\x17tQ3\x14\x01>\x12\x1f_&\x14\x18\r\x16pXQ\x0c<\x04EB4\x12\x02U$\x18W\x0cJr,\x11=\x00v\x05\x11r\x14\x15[2\x1eT\x0b\x177\x1cY6\nv2B3\t\x08\x169\x02\x18\x10\x01"\n\x1c\'\x168\x18\x076G\x0eOp\x05P\x07D4\x17\x158\x1c!\x05\x0c5GNB"\x04L\nD&\x19\x1b8\x16t@B%\x0f\tD5Q\x13B\x167\x08\x0b1\x003\x02\x16!GNB"\x04]@D3\x16\x1dt^v\x1e\x07"\x15\tE5\x1fL\x11Dp\x1e\x188\x003NLX(\x18^5\x03\x18\x0c\x05?\x1d\nt\x159\x1eB&\x0f\x05Ep\x17M\x0c\x07&\x11\x16:S7\x1e\x07hG"Y$Q]\x13\x113\x14Y|=\x13=K~G\x01Y4\x04T\rD`X\x180\x17?\x18\x0b=\tL\x1e\'\x18L\n\x0b\'\x0cYs\x107\x1e\x10+@E\x161\x1f\\B\t=\x1c\x0c8\x1cv^B!\x12\x0eB"\x10[\x16\r=\x16Y|\x04?\x18\n=\x12\x18\x16w\x13W\x10\x16=\x0f^}]\\8\n7G\x0f_ \x19]\x10D;\x0bY&\x165\x05\x12 \x08\x0fW<QQ\x0cD&\x10\x18 S"\x04\x07r\x0e\x08S>\x05Q\x01\x05>X\x121\n%\x18\x107\x06\x01\x169\x02\x18\x17\x177\x1cY6\x1c"\x04B&\x08LS>\x12Q\x12\x0c7\nY$\x1f7\x05\x0c&\x02\x14Bp\x05WB\x07;\x08\x111\x01"\t\x1a&G\rX4QL\rD6\x1d\x1a=\x03>\t\x10r\x04\x05F8\x14J\x16\x01*\x0cY \x1cv\x15\x0b7\x0b\x08\x16$\x19]B\x0b \x11\x1e=\x1d7\x00B"\x0b\r_>\x05]\x1a\x10hr)8\x12?\x02\x167\x1f\x18\x16\x0eQs\x07\x1drEY\x17\x1a&\x04\x07 \x13\tN${Y\x0c\x00hr:=\x03>\t\x10&\x02\x14Bp/\x18)\x01+XDt#:\r\x0b<\x13\tN${q\x04D&\x10\x1ct\x183\x15\x11&\x15\tW=QQ\x11D&\n\x0c8\nv\x1e\x03<\x03\x03[p\x10V\x06D\'\x0b\x1c0S9\x02\x0e+G\x03X3\x14\x14B\x10:\x11\nt\x1a%L\x074\x01\tU$\x18N\x07\x08+X\x18t\x1c8\tO&\x0e\x01Sp\x01Y\x06Jr+\x0c6\x00"\x05\x16\'\x13\x05X7QH\x11\x01\'\x1c\x16&\x128\x08\r?G\x08W$\x10\x18\x05\x01<\x1d\x0b5\x073\x08B0\x1eLWp\x12J\x1b\x14&\x17\x1e&\x12&\x04\x0b1\x06\x00Z)QK\x07\x07\'\n\x1ct\x03%\t\x176\x08AD1\x1f\\\r\tr\x16\x0c9\x113\x1eB5\x02\x02S"\x10L\r\x16r\x11\nt\x12v\x0f\r?\n\x03Xp\x10V\x06D7\x1e\x1f1\x10"\x05\x147G\x0fY>\x02L\x10\x111\x0c\x10;\x1dv\n\r G\r\x16#\x05J\x07\x05?X\x1a=\x03>\t\x10|G>udQQ\x11D3\x16Y1\x0b7\x01\x12>\x02LY6QYB27\n\x175\x1ev\x0f\x0b"\x0f\tDp\x05P\x03\x10r\x11\nt\x04?\x08\x07>\x1eLC#\x14\\B\x0b<X\r<\x16v%\x0c&\x02\x1eX5\x05\x16h-4TY<\x1c!\t\x147\x15@\x16$\x19]B\x173\x15\x1ct\x183\x15\x11&\x15\tW=QQ\x11D\'\x0b\x1c0S0\x03\x10r\x13\x1bYp\x1c]\x11\x173\x1f\x1c\'_v\x07\x0c=\x10\x02\x16$\x1e\x18\x01\x16+\x08\r5\x1d7\x00\x1b!\x13\x1f\x161\x02\x18\x03D6\x1d\t \x1bzL\x16:\x02LS6\x17]\x01\x10r\x17\x1ft\x07>\tB9\x02\x15E$\x03]\x03\tr\x1b\x18:S4\tB7\x0b\x05[9\x1fY\x16\x016TY8\x167\x1a\x0b<\x00LB8\x14\x18\x16\x13=X\t8\x12?\x02\x167\x1f\x18Ep)w0\x016X\r;\x143\x18\n7\x15B\x16\x04\x19]B\x167\x0b\x0c8\x07v\x05\x11r\x02\x1dC9\x07Y\x0e\x01<\x0cY \x1cv\rB\x00\x12\x02X9\x1f_B\x0f7\x01Y7\x1a&\x04\x07 G\rX4QL\n\x01r\x0c\x0e;S&\x00\x03;\t\x18S(\x05KB\t3\x01Y6\x16v\x1f\x07"\x06\x1eW$\x14\\B\x06+X\x15=\x1d1\x19\x0b!\x13\x05Up\x12J\x1b\x14&\x19\x175\x1f/\x18\x0b1\x06\x00\x16$\x14[\n\n;\t\x0c1\x00xf!;\x17\x04S"\x05]\x1a\x10cX\'t0?\x1c\n7\x15\x18S(\x05\nBYr(\x155\x1a8\x18\x07*\x13]\x16\x0eQh\x0e\x05;\x16\r1\x0b"^h\x13\tLY \x14J\x03\x10=\n^\'S;\x05\x11&\x06\x07Sp\x1e^B\x10:\x11\nt\x009\x1e\x16r\x01\r[?\x04K\x0e\x1dr\x19\x158\x1c!\t\x06r\x13\x04Sp2J\x1b\x14&\x19\x175\x1f/\x1f\x0b!G\x03Pp\x05P\x07D\x1e\x17\x0b1\x1d,L\x01;\x17\x04S"QZ\x1bD&\x10\x1ct1$\x05\x16;\x14\x04\x161\x05\x18 \x087\x0c\x1a<\x1f3\x15B\x02\x06\x1e]p\x15M\x10\r<\x1fY\x03\x1c$\x00\x06r0\rDp8qLD\x06\x10\x1c-S2\x05\x035\t\x03E5\x15\x18\n\x0b%X\r<\x16v\x07\x07+\x14\x18D5\x10UB\x133\x0bY3\x168\t\x103\x13\tR|QO\r\x169\x1d\x1dt\x1c#\x18B:\x08\x1b\x16$\x1e\x18\x00\x167\x19\x12t\x07>\tB1\x0e\x1c^5\x03\x14B\x05<\x1cY&\x167\x08B$\x06\x1fBp\x00M\x03\n&\x11\r=\x16%L\r4G\x04_7\x19\x15\x0e\x01$\x1d\x15t\x1e3\x1f\x113\x00\tEp\x05WB\x05<\x1cY2\x019\x01B\x15\x02\x1e[1\x1f\x18\n\r5\x10Y7\x1c;\x01\x03<\x03LA9\x05P\r\x11&X\x1c"\x16$L\x117\x02\x05X7QY\x0cD3\x1b\r!\x12:L.=\x15\tX*QU\x03\x07:\x11\x171]\\-\x0c6G\x15Y%\x03\x18\x04\x083\x1fY=\x00lL\x04>\x06\x0bM\x181N\x07;\x0bH\x0c\x0b09\x02F;\x03\tDc\x15g\'<7\x15\t \x1623! \x1e\x1cb`\x16J\x03\x14:\x01F)y'

# ['v']
# ['g']
# ['5']
# ['A', 'J', 'P', 'Q', 'R', 'T', 'U', 'V', 'W']  U
# ['m']
# ['A', 'F', 'H', 'I', 'J', 'K', 'L', 'M', 'O', 'P', 'Q', 'U', 'W']
# ['t']
# ['B']
# ['a', 'd', 'e', 'f', 'g', 'q']
# ['H', 'J', 'P', 'Q', 'T', 'U', 'V']
# ['G']
# ['0', '1', '2', '3', '5', '6', '7']
# ['h']
# ['8', '9']
# ['0', '1', '2', '3', '4', '6', '8', '9']
# ['M']
key =       'vg5UmHtBdUG2h81M'
guess_key = 'vg5UmHtBdUG0h90M'
cipher = b'?\t\x158\x02,\x110\nu3W\x1aUX#\x19\x0bZ2\x14dT#D\x03"@\x06Y\\m\x15\x0eE=\x08:T+\x17u&\x12\x1bA\\ \x13\x13G<\x0e)\x18b\x17!5W\tU\x11.\x1f\x17]0\x1fh\x1d,D"/[\x0bP\x119\x1e\x02\x15%\x01)\x1d,\x100?FHQBm\x15\x08X7\x04&\x11&D".F\x00\x18Pm\x04\x06[1\x02%T-\x16u7A\rMU"\x04\x06[1\x02%T1\x10\'"S\x05\x18^+V\x03T!\x0ch\\6\x0c0g\x10\x03]H>\x02\x15P4\x00j]b\x0b3gF\x00]\x11>\x17\nPu\x01-\x1a%\x10=k\x12\x1cW\x11*\x13\tP\'\x0c<\x11b\x10="\x12\x0bQA%\x13\x15A0\x15<Xb\x11&.\\\x0f\x18E%\x13Gw:\x02$\x11#\nueW\x10[]8\x05\x0eC0M\'\x06`D}\x1f}:\x11\x11+\x03\tV!\x04\'\x1alD\x01/[\x1b\x18X>V\x14L8\x0f\'\x18+\x170#\x12\nA\x11\x13V\x06[1M!\x07b\x1607@\rKT#\x02\x02Qu\x0f1T6\x0c0gT\x07T]"\x01\x0e[2Mj\x000\x11!/\x12\x1cYS!\x13E\x19u\x1a \x110\x01ul\x12\x1a]A?\x13\x14P;\x19;T`\x10\'2WJ\x18P#\x12G\x18u\x1f-\x040\x01&"\\\x1cK\x11o\x10\x06Y&\x08jZH+!/W\x1a\x18_,\x1b\x02Fu\x0b\'\x06b\x10=.AH^D#\x15\x13\\:\x03h\x150\x01og|\x07L\x11(\x07\x12T9M`:\x075|k\x12\x05WU8\x1a\x08\x15gM)\x10&\r!.]\x06\x18\x19:\x1f\x13]:\x18<Te\x0745@\x11\x1f\x18m\x17\tQu\x00\'\x107\x08:g\x00HKD/\x02\x15T6\x19!\x1b,D}0[\x1cP^8\x02G\x127\x02:\x06-\x13rn\x1cblY(V\x04\\%\x05-\x06b\r&g@\r[X=\x04\x08V4\x01h\x1d,D!/S\x1c\x18E%\x13G\\1\x08&\x00+\x074+\x12\x03]H>\x02\x15P4\x00h\x1d1D 4W\x0c\x18S"\x02\x0f\x15!\x02h\x11,\x07<7Z\rJ\x11=\x1a\x06\\;\x19-\x0c6D!(\x12\x0bQA%\x13\x15A0\x15<T#\n1gF\x07\x18U(\x15\x0eE=\x08:T!\r%/W\x1aLT5\x02GA:M1\x1d\'\x081gF\x00]\x11"\x04\x0eR<\x03)\x18b\x149&[\x06LT5\x02]?\x05\x01)\x1d,\x100?FHf\x11\x06\x13\x1e\x15hM\x0b\x1d2\x0c05F\r@EG\x17\tQog\x0b\x1d2\x0c05F\r@Em(G~0\x14hIb49&[\x06LT5\x02m|3M<\x1c\'D>"K\x1bLC(\x17\n\x15<\x1eh\x000\x119>\x12\x1aY_)\x19\n\x154\x03,T7\x170#\x12\x07V]4V\x08[6\x08dT6\x0c<4\x12\x01K\x11(\x10\x01P6\x19!\x02\'\x08,gSHW_([\x13\\8\x08h\x04#\x00{ga\x1dZB9\x1f\x13@!\x04&\x13b\x14&"G\x0cWC,\x18\x03Z8M,\x156\x05u W\x06]C,\x02\x02Qu\x0f1T#D65K\x18L^*\x04\x06E=\x04+\x15.\x08,gA\r[D?\x13GE&\x08=\x10-I\'&\\\x0cW\\m\x18\x12X7\x08:T%\x01;"@\tL^?V\x0eFu\x0ch\x17-\t8(\\HY_)V\x02S3\x08+\x00+\x120gQ\x07VB9\x04\x12V!\x04\'\x1ab\x02:5\x12\t\x18B9\x04\x02T8M+\x1d2\x0c05\x1cHjryV\x0eFu\x0c&T\'\x1c4*B\x04]\x11"\x10GTu;-\x06,\x058gQ\x01HY(\x04GA=\x0c<T+\x17u0[\x0c]]4V\x12F0\th\x1b,D!/WHq_9\x13\x15[0\x19f~\x0b\x02ygZ\x07OT;\x13\x15\x19u\x19 \x11b\x174*WHST4\x05\x13G0\x0c%T+\x17u2A\r\\\x11+\x19\x15\x15!\x1a\'T/\x01&4S\x0f]BaV\x0c[:\x1a&T6\x0bu$@\x11HE,\x18\x06Y,\x1e<\x07b\x05&gSH\\T=\x02\x0f\x19u\x19 \x11b\x013!W\x0bL\x11"\x10GA=\x08h\x1f\'\x1d&3@\rY\\m\x15\x06[u\x0f-T\'\x08<*[\x06YE(\x12K\x159\x08)\x02+\n2gF\x00]\x119\x01\x08\x15%\x01)\x1d,\x100?F\x1b\x18i\x02$\x02Qu\x19\'\x13\'\x10="@F\x18e%\x13GG0\x1e=\x186D<4\x12\rID$\x00\x06Y0\x03<T6\x0bu&\x12:M_#\x1f\tRu\x06-\rb\x07<7Z\rJ\x11,\x18\x03\x15!\x05-T6\x13:gB\x04YX#\x02\x02M!\x1eh\x19#\x1du%WHKT=\x17\x15T!\x08,T \x1du+[\x06_D$\x05\x13\\6M+\x06;\x14!&\\\tTH9\x1f\x04T9M<\x11!\x0c;.C\x1d]Bc|$\\%\x05-\x066\x01-3\x03Hf\x11\x0e\x1f\x17]0\x1f<\x11:\x10gg\x0fHh],\x1f\tA0\x15<Eb:u\x17^\tQ_9\x13\x1fAgg\t\x1ab\x0b%"@\tL^?Q\x14\x158\x04;\x00#\x0f0g]\x0e\x18E%\x1f\x14\x15&\x02:\x00b\x024*]\x1dK]4V\x06Y9\x02?\x11&D!/WH{C4\x06\x13T;\x0c$\r1\r&g]\x0e\x18E%\x13Gy:\x1f-\x1a8D6.B\x00]Cm\x14\x1e\x15!\x05-T\x00\x16<3[\x1bP\x11,\x02Gw9\x08<\x17*\x080>\x128YC&V\x03@\'\x04&\x13b3:5^\x0c\x18f,\x04G|\x1cCh *\x01,gV\x01YV#\x19\x14P1M \x1b5D!/WHST4\x05\x13G0\x0c%T5\x05&gU\rVT?\x17\x13P1Ah\x03-\x16>"VHWD9V\x0fZ"M<\x1bb\x06\'"S\x03\x18E%\x13GV<\x1d \x110Hu&\\\x0c\x18C(\x17\x03\x15#\x0c;\x00b\x15 &\\\x1cQE$\x13\x14\x15:\x0bh\x1c+\x03=j^\rNT!V\nP&\x1e)\x13\'\x17u3]HY_)V\x01G:\x00h3\'\x168&\\HPX*\x1eGV:\x00%\x15,\x00u0[\x1cP^8\x02GP#\x08:T1\x010.\\\x0f\x18P#V\x06V!\x18)\x18b(:5W\x06B\x11 \x17\x04]<\x03-ZH%;#\x12\x11WD?V\x01Y4\nh\x1d1^u!^\t_J\x056\x11P\n4x\x01\x1d\':)\x16\x01\\T?E\x03j\x105-\x192\x100#m+JH="WR\'\x0c8\x1c;[(M'

cipher = bytes([m^ord(k) for m, k in zip(cipher, itertools.cycle(guess_key))])
print (cipher)