/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - iga -
 *
 *                         Version 0.1.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : tex.h
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Mar,26,1999     okadaa    1st version
 */

unsigned int tex_data[] __attribute__((aligned(16))) = {
	0x00000004, 0x10000000, 0x0000000e, 0x00000000, /* GIFtag(Packed) */
	0x00000000, 0x02012980, 0x00000050, 0x00000000, /* BITBLTBUF */
	0x00000000, 0x00000000, 0x00000051, 0x00000000, /* TRXPOS */
	0x00000008, 0x00000002, 0x00000052, 0x00000000, /* TRXREG */
	0x00000000, 0x00000000, 0x00000053, 0x00000000, /* TRXDIR */
	0x00000002, 0x08000000, 0x00000000, 0x00000000, /* GIFtag(Image) */
	0x21087fff, 0x318c294a, 0x3def35ad, 0x4a524631, /* CLUT data */
	0x56b55294, 0x63185ef7, 0x6f7b6739, 0x8000739c, /* CLUT data */
	0x00000004, 0x10000000, 0x0000000e, 0x00000000, /* GIFtag(Packed) */
	0x00000000, 0x14042580, 0x00000050, 0x00000000, /* BITBLTBUF */
	0x00000000, 0x00000000, 0x00000051, 0x00000000, /* TRXPOS */
	0x00000080, 0x00000080, 0x00000052, 0x00000000, /* TRXREG */
	0x00000000, 0x00000000, 0x00000053, 0x00000000, /* TRXDIR */
	0x00008200, 0x08000000, 0x00000000, 0x00000000, /* GIFtag(Image) */
	0xdddccccc, 0xeeeeeeee, 0xeeeeeeee, 0xebbcdeed, /* Index data */
	0xcccbde00, 0x94cea57c, 0xceedbbde, 0x459cbccc,
	0xaaaa9744, 0x88888999, 0xbcccba98, 0xbbaa999a,
	0xce0edccb, 0x0eba8557, 0xeeee99d0, 0xcdcccb9e,
	0xdccccccc, 0xeeeeeedd, 0xeeeeeeee, 0xdbccdedd,
	0xdccbde00, 0x94aec75a, 0xceedbace, 0x5799aacc,
	0x99864224, 0x98877789, 0x99999999, 0xeedccaa9,
	0x47ce000e, 0xe00eb864, 0xeeeec89c, 0xddcccc9b,
	0xcccccccc, 0xeeeedddc, 0xeeeeeeee, 0xcbcceede,
	0xdcccde00, 0xa49ec958, 0xddedbace, 0x799aaabc,
	0x98765456, 0x9aba8777, 0xbaaa9999, 0x0000edcc,
	0xa9ad0000, 0xbde000ec, 0xbddeec99, 0xdcccccb9,
	0xcccddddd, 0xeeedcccc, 0xeeeeeeee, 0xcbcdeeee,
	0xdcccce0e, 0xb49eca57, 0xddeebace, 0xbbaabbac,
	0x8977889a, 0xcbbdd976, 0xeddccccc, 0x0000000e,
	0x00000000, 0xbabbcee0, 0xbabcdeec, 0xdccccacc,
	0xcddeeeee, 0xeddccbcc, 0xeeeeeeee, 0xbccdeeee,
	0xbdccce0e, 0xc48dab75, 0xcdeecace, 0xccbbbbbb,
	0x5997799b, 0xeebaed97, 0x00000000, 0x00000000,
	0xe0000000, 0xdedca9ac, 0xbbbbbccc, 0xdccccaab,
	0xdeeeeeee, 0xddcbbbcc, 0xeeeeeeee, 0xbcdeeeee,
	0x9dccce0e, 0xc47d9a85, 0xcdeecabe, 0xabbbbbcb,
	0x57997999, 0x00e9dea8, 0x00000000, 0x00000000,
	0xace00000, 0xbbceec98, 0xccbbbbcb, 0xdccccbcc,
	0xeeeeeeee, 0xcccabccd, 0xeeeeeeed, 0xbcdeeeee,
	0x7dccce0d, 0xc47d9995, 0xbcdecabe, 0xaaaabccc,
	0x669977aa, 0x00eaeeb8, 0x00000000, 0x00000000,
	0x79ce0000, 0xcb9be0ea, 0xbbccbbbc, 0xdcccccbb,
	0xeeeeeeee, 0xccaabcde, 0xeeeeeeec, 0xbcdeeeee,
	0x5cccce0c, 0xc56cb995, 0xccdedabd, 0xcccbaccc,
	0x7599879c, 0x00eaeec9, 0x00000000, 0x00000000,
	0xa79bce00, 0xbcc9ad0e, 0xcabbccbb, 0xdccccbbc,
	0xeeeeeeee, 0xcbaabcce, 0xeeeeeedc, 0xbceeeeee,
	0x4adcceec, 0xc55ccb97, 0xcbcedabd, 0xccccabcc,
	0x669a9769, 0x00ca0eb9, 0x00000000, 0x00000000,
	0xe969ace0, 0xbbcc99e0, 0xeecbaccb, 0xccccbcee,
	0xeeeeee0e, 0xcbaaabcd, 0xeedeedcc, 0xcdeeeeee,
	0x48ecceec, 0xd55ccc98, 0xcbcedbad, 0x79ccabcd,
	0x579a9865, 0x0e9c0db9, 0x00000000, 0x00000000,
	0x0c67aace, 0xbabca8be, 0xceecbacb, 0xcbcccdcc,
	0xdeeeee00, 0xcbaaabcc, 0xedeeedcc, 0xcdeeeeee,
	0x57dccdec, 0xd55cdea9, 0xdccddbad, 0x459cbbcd,
	0x599bb786, 0x0baeeca9, 0x00000000, 0xce000000,
	0x0e958abb, 0xbbacc98c, 0xcbdecbbc, 0xcbcccccc,
	0xcddeeee0, 0xccbbbbcc, 0xddeedccc, 0xcdeeeeee,
	0x65cdcdec, 0xd55cdec9, 0xdccdebad, 0x7779bbce,
	0x5a8bc878, 0xc9d0ebb7, 0x00000000, 0xbbcde000,
	0xceb569ab, 0xcbabdb79, 0xecbdebac, 0xcbccccee,
	0xcccdeeee, 0xccbbcccc, 0xddeecccc, 0xcdeeeeee,
	0x74aeccec, 0xd55cded9, 0xcccdebad, 0x9987abcd,
	0x7b7bca77, 0x9c0ecba5, 0xe00000eb, 0xaaaabcce,
	0x9cb7569a, 0xcbbacc97, 0xeeccedbb, 0xbbddcdcd,
	0xcccccdee, 0xcbbbcccc, 0xdeedcccc, 0xceeeeeed,
	0x849eccec, 0xd54ceeea, 0xcccdebac, 0x69a79cdc,
	0x9b7bbb97, 0xd0ecbc75, 0xbccdcc99, 0x9aabbaaa,
	0x79975579, 0xcbbacdc8, 0xceeccecb, 0xbcdcc978,
	0xcccccccd, 0xcccccccc, 0xdeeccccc, 0xceeeeeed,
	0x957eccec, 0xd54ceeeb, 0xccccebac, 0x58b79ccc,
	0xc97babb8, 0x00dcc946, 0xba999ace, 0x999bcccc,
	0x76887557, 0xbcbbbdca, 0x7ceccecb, 0xbccc9787,
	0xcccccccc, 0xcccbbbcb, 0xeedcccbb, 0xceeeeedd,
	0x955cdcec, 0xd64ceeec, 0xccccebac, 0x66b89ccc,
	0xd87c9aca, 0xedccb559, 0xeeddee00, 0x7999bcce,
	0x97677656, 0xacbbacdc, 0x79eecdeb, 0xacc98bcb,
	0xcccccccc, 0xbbabbccc, 0xedccccbb, 0xceeeeedd,
	0x974cdcdc, 0xd64ceeee, 0xdcccebac, 0x76b7accd,
	0xc68c99cb, 0xcccc557c, 0xeee000ee, 0x77999abc,
	0xb9656656, 0xbcbbbbdd, 0xb7ceccec, 0xbca8bcde,
	0xcccccccc, 0xaabccccc, 0xedccccba, 0xceeeeede,
	0xa749dcdc, 0xe74beede, 0xdccceb9c, 0x77a7bcce,
	0xb59b9acb, 0xccc6569e, 0xcdeeeedc, 0xa77999ab,
	0xcb965568, 0xbbcabacd, 0xd8aecced, 0xcc89cdee,
	0xcccccccc, 0xbccccccc, 0xecccccbb, 0xceeeedde,
	0xc948dcdc, 0xe73beece, 0xdcccec9c, 0x6898ccdd,
	0x77cb9bcb, 0xcc7577ce, 0xccdccccc, 0xc967a9aa,
	0xdca96569, 0xbacbbacd, 0xe99edcde, 0xc98bcedd,
	0xcccccccc, 0xcccccccc, 0xeccccccc, 0xceeeedee,
	0xc956ccdc, 0xe73bedcd, 0xdcbcec9c, 0x697acddd,
	0x69cbacc9, 0xc7577beb, 0xcddddccc, 0xbc968bab,
	0xecba9757, 0xcbccbbbc, 0xdc8decce, 0xa79ceecc,
	0xcccccccc, 0xcccccccc, 0xecccdddc, 0xceeeedee,
	0xda64bccc, 0xe73aedcc, 0xdcbcec9c, 0x889ccddd,
	0x9cbbccb7, 0x5777aec7, 0xbcdcccc8, 0x7cc859b9,
	0xddcba975, 0xcbbcabbc, 0xcd8cecce, 0x78ceeeec,
	0xdccccccc, 0xcccccccc, 0xdccccccc, 0xceeeedee,
	0xdb749bcc, 0xe739ddcc, 0xdcbcec9b, 0x89bcdddc,
	0xbcbccb87, 0x887aec78, 0xa99a9877, 0x59cc759b,
	0xceccbaa7, 0xebbcbbbb, 0xcd9beccd, 0x7aeee0ee,
	0xdddddccc, 0xcccccddd, 0xdccccccc, 0xceeeedee,
	0xcc947bcc, 0xe839ddcb, 0xdcbcec9b, 0x9bcdddcc,
	0xccccb977, 0x78cec89b, 0x9aa99998, 0x879cc867,
	0xcddccaba, 0xecbbcacb, 0xdc99edcc, 0x9de0000e,
	0xdddddddd, 0xcccddddd, 0xdccccccc, 0xcdeeedee,
	0xbca55acc, 0xe829dcca, 0xccbcec9a, 0xbccccccd,
	0xccb98789, 0xacdb99bc, 0x77888878, 0xb877acb9,
	0xbcedcbab, 0xecbbcabb, 0xeca9edcc, 0xd0000000,
	0xeeeedddd, 0xcccddeee, 0xcccbbbcc, 0xcdeeedee,
	0xbcb749bc, 0xe937cccb, 0xccbcec9a, 0xcccaaccd,
	0xca8779ac, 0xdc99accc, 0xca99abcd, 0xcb9767ac,
	0xbbddccba, 0xcdbbccbc, 0xedb8decc, 0x00000000,
	0xeeeeeeee, 0xccddeeee, 0xccbabbcc, 0xcceeedee,
	0xccc947bc, 0xe937cccc, 0xccaced9a, 0xcb99accd,
	0x9878accc, 0xa9abccca, 0x9ccddddc, 0xabba8767,
	0xcbcddccb, 0xcecbbcac, 0x0dc8cecc, 0xeede0000,
	0xeeeeeeee, 0xcddeeeee, 0xcbbbbccc, 0xcceeedee,
	0xdcca559c, 0xda36cccc, 0xccaceda9, 0xa999acce,
	0x99abccbb, 0xabcccb99, 0x7899aaaa, 0xbaabba87,
	0xccbcedcc, 0xbdcbbcbb, 0x0ec8cecc, 0xbbcce000,
	0xeeeeeeee, 0xddeeeeee, 0xdcbbbccc, 0xcceeedee,
	0xedcb847a, 0xdb45bccc, 0xccaceda9, 0xa999bbce,
	0xbbbbaaaa, 0xcccb99aa, 0xa999aabb, 0xccaabcba,
	0xccbbcedc, 0xacdbbbcb, 0x0ec8becc, 0x89acceee,
	0xeeeeeeed, 0xeeeeeeee, 0xdcccccde, 0xbcdeedee,
	0xeecca558, 0xcc44accc, 0xccaceea9, 0xca9abbdd,
	0xbbbccccc, 0xbba9abbb, 0xcccccccc, 0xccccbabb,
	0xbccbbcdd, 0xbaeccacc, 0x0ec9aedc, 0xaa9acccd,
	0xeeeededd, 0xeeeeeeee, 0xedccddee, 0x9cceedee,
	0xe0ecb847, 0xcc539ccc, 0xccaceeb9, 0xc99bbced,
	0xcccccccd, 0xaaaabbbc, 0xbbbbbbba, 0xedccccbb,
	0xbcccabcd, 0xc9cdbbbc, 0xeed99eec, 0xcb99ccbc,
	0xeeeeeddd, 0xeeeeeeee, 0xeddddeee, 0x9cceedee,
	0xe0ecca55, 0xbd627ccc, 0xdcaceeb9, 0xb9abacec,
	0xeeedcccc, 0xaabbccdd, 0xbbbaaaaa, 0xdeddcccc,
	0xcbcccabc, 0xcbbebcac, 0xded99eec, 0xcba99cbb,
	0xddddeddd, 0xeeeeeeee, 0xeeddeeee, 0x7bcdeeee,
	0xe00ecb85, 0xad726bcc, 0xdc9ceec9, 0x9ababeec,
	0xeeedcccb, 0xcccccccd, 0xcccbbbbb, 0xccddedcc,
	0xcbccccab, 0xccbdcbba, 0xdeda8dec, 0xccb98bcc,
	0xdddddddd, 0xeeeeeeee, 0xeeeeeeee, 0x59cceeee,
	0xe000dca5, 0x9d924acc, 0xdbabeec9, 0xabaacecd,
	0xeeecccb9, 0xccccbbce, 0xdccccccc, 0xacccdddd,
	0xccbcccca, 0xbcccebca, 0xceda7cec, 0xdca979cc,
	0xdddddddd, 0xeeeeeeee, 0xeeeeeeee, 0x47aceeee,
	0xe000ecc8, 0x9cc339cc, 0xebabeeda, 0xbaaceece,
	0xeddccb9a, 0xcccaabce, 0xdddccccc, 0xbabccccd,
	0x9ccbdccc, 0xbcccecbb, 0xcedb7ced, 0xeca989cc,
	0xddddddee, 0xeeeeeeed, 0xeeeeeeee, 0x558cdeee,
	0xe0000ecb, 0x9bd429cc, 0xdbaadeeb, 0xa9bdedce,
	0xddcbaaab, 0xcbabccee, 0xccdddddc, 0xcbaacccc,
	0xabdccdcc, 0xbcccceab, 0xcedc7bed, 0xecb989ad,
	0xddddeedd, 0xeeeeeddd, 0xeeeeeeee, 0x8469ceee,
	0xd00000ec, 0x9ad627cc, 0xcabaceec, 0x9aceecde,
	0xcbaaabba, 0xbbccdddc, 0xcccccccb, 0xcccaabcc,
	0xb9ccccdc, 0xcbccceca, 0xceec79de, 0xeddb899c,
	0xddeeeeee, 0xeeeeeddd, 0xeeeeeeee, 0xb547ceee,
	0xe000000d, 0x99d827cc, 0xcab9ceec, 0xaceedeee,
	0xaaabcba9, 0xcccdccba, 0xcccccccc, 0xdccccaab,
	0xbb9ccccd, 0xcbccccea, 0xceec89dd, 0xeeed999b,
	0xdeeeeeee, 0xeeeeeddd, 0xeeeeeeee, 0xd9459dee,
	0xe0000000, 0xa9c926cc, 0xaab9beed, 0xcdeeeeec,
	0xabccb99a, 0xccccbaaa, 0xaabccccc, 0xddccccba,
	0xacb9cccc, 0xdbccbcec, 0xceec98cd, 0xeeeeba9a,
	0xeeeeeeee, 0xeeeeedde, 0xeeeeeeee, 0xec747cee,
	0xe00eeee0, 0xb9ca26cc, 0x9bb9adee, 0xdeeeeeca,
	0xccba99ac, 0xbaa99abb, 0xbaaaaabb, 0xcdedcccc,
	0xbaca9dcc, 0xdbccbbce, 0xceec97cd, 0xedeecba9,
	0xeeeeeeee, 0xeeeeedde, 0xeeeeeeee, 0x0eb559de,
	0xe00dcbbd, 0xc9bb27cd, 0xbba99dee, 0xeeeedca9,
	0xcaaaaacd, 0xaaabcccc, 0xcccbbaaa, 0xcccedccc,
	0xeabbaacc, 0xdcbccbcd, 0xceeca7cd, 0xeccedcc9,
	0xeeeeeeee, 0xeeeeedee, 0xeeeeeeee, 0xceea57ce,
	0xe0dbaa99, 0xc9ac37cd, 0xcac87cee, 0xeeedca9a,
	0xaaabbcde, 0xcccccccb, 0xccdddccc, 0xcccceedc,
	0xed9bba9c, 0xdcacccbc, 0xbeecb7ad, 0xebadecca,
	0xeeeeeeee, 0xeeeeedee, 0xeeeeeeee, 0x9aee959d,
	0xeecbcccb, 0xd9ab37de, 0xabc58dde, 0xeecba99c,
	0xbbbccdee, 0xcccccbbb, 0xdccccddc, 0xccccceee,
	0xcec9bba9, 0xddaaccbb, 0xadecc79e, 0xecbdedca,
	0xeeeeeeed, 0xeeeeeeee, 0xeeeeeeee, 0xcbaed77c,
	0xecbcdddd, 0xda9b38de, 0x9cb2adce, 0xdcbaa9bc,
	0xbbccdeee, 0xcccccbcb, 0xedcccccc, 0x9cdcccde,
	0xbcebabba, 0xdec9bcbb, 0xadecc88d, 0xeecdedcb,
	0xeeeeeded, 0xeeeeeeee, 0xdeeeeeee, 0xedcbdc79,
	0xeccdeeee, 0xea9b49de, 0xac92cdce, 0xcbbbabca,
	0xbcceeeec, 0xcccccccb, 0xeeedcccc, 0xa9cdcccc,
	0xabdeaabb, 0xeeda9ccb, 0xacecc97c, 0x0eceedcc,
	0xeeeeeddd, 0xeeeddeee, 0xbeeeeeee, 0xeeecbdc7,
	0xccdeeeee, 0xeb9a4aee, 0xcd83dcce, 0xbccbbca9,
	0xcdeeedcb, 0xcccccccc, 0xcceeeddc, 0xba9ceccc,
	0xbabeeaac, 0xeeec9acc, 0xbcedca7a, 0x0eceeccc,
	0xeeeeeddd, 0xeedddeee, 0x9cedeeee, 0xeeeedcd9,
	0xcceeeeee, 0xeb9a4bee, 0xcd54ecce, 0xcdccca9b,
	0xeeedccbc, 0xdddcccdd, 0xcccdeeee, 0xbba9becb,
	0xcbaceeaa, 0xeeeca9bc, 0xbcedcb78, 0x0ecdeccd,
	0xeeeeeddd, 0xeeedddee, 0x8addeeee, 0xcceeedcc,
	0xcdeeeccc, 0xec995ced, 0xcc26eccd, 0xeeddb99c,
	0xedcccccd, 0xeeeeeeee, 0xbccccdee, 0xbbba9aed,
	0xdcbbced9, 0xbeeeb99c, 0xcceecc87, 0x0ebddabd,
	0xeeeedddd, 0xeeedcdee, 0xb8cedeee, 0xbbcceedd,
	0xceeecbab, 0xec995cec, 0xdc17ecbd, 0xeedc99bc,
	0xcccddeee, 0xddeeeddc, 0xecbccccc, 0x9bbbb9ac,
	0xcdcbbcec, 0x8eeeca9a, 0xcbeecc97, 0x0ebcdabd,
	0xeeeedddd, 0xeeedcdde, 0xc99ddeee, 0xcccccded,
	0xdeecabbc, 0xec995cec, 0xdc17ebac, 0xeec99bcc,
	0xcdeeeeee, 0xcccccccc, 0xcedbaccc, 0xc9bbbb99,
	0xccdcabce, 0x7b0eec99, 0xcbd0ccc7, 0x0eb9cbcd,
	0xeeeeeddd, 0xeeeedcdd, 0xdc7bdeee, 0xddddccee,
	0xdedabccc, 0xec995dec, 0xd919ebac, 0xec99bccc,
	0xeeeeeeee, 0xcdddddde, 0x9adeca9b, 0xeb9cbbb9,
	0x9cccbabd, 0x78e0eda9, 0xcbc0cbd9, 0xeec9cccd,
	0xeeeeedcd, 0xeeeedddd, 0xec98ddee, 0xeeeeddce,
	0xeecaccdd, 0xed996dec, 0xc41cebac, 0xca9accdd,
	0xeee0000e, 0x9acdeeee, 0x999ceca9, 0xeea9cbbb,
	0x9bcccbbb, 0x77b0eec9, 0xcbc0eadb, 0x9ee9accd,
	0xeeeeeddd, 0xeeeeedde, 0xedc7adde, 0xeeeeeedd,
	0xeecbccde, 0xed996dec, 0xa13ddb9b, 0xb99accdd,
	0xe000000e, 0x9779acce, 0xba98accb, 0xbeeaacab,
	0x99ccdcbb, 0x879e0eda, 0xccceeacc, 0x78c99ccc,
	0xeeeeedcd, 0xeeeeeedd, 0xdeda7cde, 0xeeeeeeee,
	0xeebbcdee, 0xeea96dec, 0x917eda9b, 0xc999ccdd,
	0xe000000e, 0xb975679b, 0xbca9989c, 0xbbed99ca,
	0x99adcdcb, 0xa77b0edc, 0xcccd0cad, 0xc7577ddc,
	0xeeeeedcd, 0xeeeeeedd, 0xedec98dd, 0xeeeeeeee,
	0xeebbcdee, 0xeea96cec, 0xc27eca9a, 0xda99bccd,
	0xce000000, 0x89755568, 0xabca9977, 0xbbced9ac,
	0xa99cdddc, 0xc789eeec, 0xcccc0e9c, 0xe9447ddc,
	0xdeeeedcc, 0xdeeeeeed, 0xedeec79d, 0xeeeeedde,
	0xeebbcdee, 0xeeaa6cec, 0xd56eda9a, 0xca99accd,
	0xbe00000e, 0x65554457, 0xcabca998, 0xbbacec9a,
	0xb999dced, 0xd987ceed, 0xcccceeab, 0xec637ddd,
	0xdeeeedcc, 0xdeeeeeed, 0xeededb7b, 0xeeeeeedd,
	0xeebacdee, 0x0eba6bec, 0xd75eda9b, 0xb999bccd,
	0xbe000eec, 0x98755557, 0xbcabcb99, 0xcbbadec9,
	0xc998bdde, 0xda779ded, 0xcbbce0ba, 0xee849eec,
	0xddeeedcc, 0xcdeeeeee, 0xddededa7, 0xeeeeeedc,
	0xdecacdee, 0x0eba7aec, 0xd94cda9b, 0xaaabcccd,
	0xceeeecba, 0x99a9999b, 0x9ccabcb9, 0xdcbbadec,
	0xca999ccd, 0xdc887bed, 0xcbbcd0c9, 0xeeb9deec,
	0xedeeedcc, 0x7cdeeeee, 0xcddeded9, 0xeeeeeeed,
	0xcecacdee, 0x00ca78ec, 0xd94cca9b, 0xbbccccdd,
	0xccccbabb, 0xa99abbcc, 0xa9ccabcc, 0xddbbbbee,
	0xcb998bcc, 0xdc9878cc, 0xcbbcd0c9, 0x0eeeeeec,
	0xeddeedcc, 0x88deeeee, 0xcccdedec, 0xeeeeeeed,
	0xcedabcee, 0x00ca97ed, 0xda3bc99b, 0xcccccddc,
	0xaaaaabbb, 0xca9999aa, 0xd99cbabc, 0xcdcbbace,
	0xcc9999dc, 0xcd9786bc, 0xccbcd0c9, 0x000eeeec,
	0xeedeeecc, 0xc79deeee, 0xdcccdede, 0xeedeeeee,
	0xcdebbcee, 0xe0d9a6ce, 0xea3ac989, 0xcccdddcc,
	0x9999abbc, 0xccba9999, 0xec8bcbab, 0xccccbbad,
	0xdc9998cd, 0xceb7968c, 0xccccd0c9, 0xeeeeeeec,
	0xeeeeeecc, 0xec89deee, 0xedcdcded, 0xeeddee0e,
	0xccecbcde, 0xe0eab79e, 0xea3ac989, 0xcccddccd,
	0x999bccbc, 0xbcccba99, 0xeeb8ccba, 0xcdccbbbb,
	0xdc9997ad, 0xbec8785a, 0xcbccd0d9, 0xdddeeeca,
	0xeeeeeedc, 0xeec79dee, 0xedccddde, 0xeeedcde0,
	0xecedbcde, 0xe0eba96d, 0xeb49d979, 0xcccdddcd,
	0xaabbcccc, 0xbaccccba, 0xcee99ccb, 0xdccccbca,
	0xbb89a88c, 0xaec96967, 0xcacbd0e9, 0xdeeeec9a,
	0xeeeeeedc, 0xddec7ade, 0xeedccddd, 0xeeeddcde,
	0xeccecbce, 0xd00c9b6a, 0xec48d978, 0xccccedcd,
	0xcccccccc, 0xbbaccccc, 0xbdec8acc, 0xdcdccbbb,
	0x9b889979, 0x9deb7885, 0xc9cbd0e9, 0xeeec979c,
	0xeeeeeeec, 0xddeeb7be, 0xeeecccdd, 0xeeeeeddd,
	0xdecedbcd, 0xc00eba97, 0xde75d977, 0xccccdedc,
	0xcccccccc, 0xbbbbbccc, 0xaceeb8bc, 0xcdccccbc,
	0x58779977, 0x8beca697, 0xa9dbc00b, 0x997779cd,
	0xeeeeeeed, 0xdddeeb7c, 0xceedccdd, 0xeeeeeeed,
	0x9ecdeccd, 0xae0ec9b7, 0xcec4ab67, 0xcccddddc,
	0xcccccccc, 0xcbbbabcc, 0xbadee99c, 0x9dcdccbb,
	0x55679997, 0x89dec879, 0x7ceac00d, 0x999bcdeb,
	0xdeeeeeed, 0xddddee99, 0xdcdedccd, 0xeeeeeddd,
	0x7ddcecbc, 0x9e00eba9, 0xcde76c77, 0xcccdeddd,
	0xcccddccc, 0xccaccabc, 0xcbceec7a, 0x7bdcdcca,
	0x95558997, 0x97bedb78, 0x9edac00e, 0xcccddca7,
	0xbeeeeeee, 0xdeddded7, 0xedccedcc, 0xdeeeeddd,
	0x79ecdebc, 0x8d00ed9b, 0xcced5a96, 0xdcccdedd,
	0xbccccddd, 0xcdbbccab, 0xbbbdee98, 0x69ccdccb,
	0x97447888, 0xb79dec96, 0xdedac000, 0x99888779,
	0x8ceeeeee, 0xcdedddeb, 0xddcbcedc, 0xceeeeddd,
	0xa7ddcecb, 0x7b000eca, 0xccdea6b7, 0xdddddded,
	0xabbccccc, 0x9ccbbcca, 0xabbceec7, 0x77bcddcc,
	0x69757978, 0xc77beec9, 0xeec9c000, 0xeedcccde,
	0x99deeeee, 0xccdeddde, 0xdddcacee, 0xbdeeeedd,
	0xb99eccec, 0x78e00eeb, 0xeccee97b, 0xccccddee,
	0xaaabbccc, 0x7cdcbbcc, 0xbbbbdee9, 0x769ccdcc,
	0x96a76976, 0xe868ceeb, 0xedb9c000, 0xbcdeeeee,
	0xc8ceeeee, 0xeccdedde, 0xdddcb9bd, 0xccdeeedc,
	0xab7becee, 0xa7a000ec, 0xeedcee89, 0xcccccccd,
	0xca9ababc, 0x79dcbbbc, 0xbbbaceec, 0x767ccddc,
	0xa7897975, 0xe967aeec, 0xcb9ae000, 0xcaabcddd,
	0xeb8deeee, 0xdedcdedd, 0xcdddca9b, 0xdbceeeec,
	0xcaa7ddce, 0xa97d00ee, 0xcdedced8, 0xbbccbccc,
	0xccb9abaa, 0x98cdcbbb, 0xcabaceed, 0x576accdc,
	0xb96a9894, 0xe9669cee, 0x999c0000, 0xedcaaaaa,
	0xdd99eeee, 0xacedceed, 0xccddccb9, 0xeccdeeec,
	0xebb99ecc, 0x9a99e0ee, 0xcccddded, 0xaaabcccc,
	0xbccb99ab, 0xb7adccab, 0xcbbabdee, 0x4758ccdd,
	0xea97a9a6, 0xd8757cee, 0x9ace0000, 0x0eedcb99,
	0xded8beee, 0xa9cedcee, 0xccdddccb, 0xdeccdeed,
	0xedbb8aec, 0xeaa99eee, 0xbcccccce, 0xbbaaabcc,
	0xbbbcb99a, 0xd88cdcba, 0xcbbbace0, 0x3556bcdd,
	0xeca78ba7, 0xc7757bde, 0xcde00000, 0xe00eeedc,
	0xedeb8cee, 0xba9bedde, 0xcccccccb, 0xceeccdee,
	0xeecab8ce, 0xeeaa9aee, 0xccbccccc, 0x9cbaaaac,
	0xbbbbcb99, 0xe97cdcba, 0xdcbbabe0, 0x45659ccd,
	0xeeba7ac9, 0x97679bde, 0xe000000e, 0xce000eee,
	0xedde99ee, 0x99a9bddd, 0xdcccccba, 0xdcedcdde,
	0xeeecaa8d, 0xcdeba9be, 0xbccbcccc, 0x99ccaaa9,
	0xaabbbcba, 0xeb7addcb, 0xdcbb9ad0, 0x44757bcd,
	0xeeca98cb, 0x86599bc0, 0x0000000d, 0xbce00000,
	0xdeded9be, 0x99999add, 0xdccccbaa, 0xecceccde,
	0xeeedcb99, 0xbbcdb99b, 0x9acccbcc, 0xa99ccaaa,
	0xaaabbccb, 0x0c79cdcb, 0xdcbba9ce, 0x626659bc,
	0xeeeaa7ac, 0x757aacc0, 0x0000000c, 0xbbce0000,
	0xddeded9c, 0xaa99999c, 0xecccccba, 0x9ecdeccd,
	0xaeeedcb9, 0xccbceb99, 0xaa9accbb, 0xba99ccaa,
	0xaabbbacc, 0x0d97cdcc, 0xddcba9ce, 0x924757ac,
	0xeeec998c, 0x559abce0, 0x000000ea, 0xcbbcd000,
	0xcdeedec9, 0xaaaaa999, 0xddcbcccb, 0x9aecedcc,
	0x9beeedbb, 0xbcccdeb9, 0xaaa99cdc, 0xcba99bca,
	0xbaacbaac, 0x0e97cddc, 0xcecba9be, 0xc437669b,
	0xeeee998a, 0x5699cce0, 0x000000e9, 0xccbbbce0,
	0x9cdeedeb, 0xcbaaaaa9, 0xcddbbccc, 0xb9cdcecc,
	0x99beeecb, 0xcbbcdeeb, 0xaaaa99ce, 0xccba99bc,
	0xbaabcbab, 0x0eb7bddc, 0xcddba9ad, 0xc725758a,
	0xdeee98a8, 0x689acd0e, 0x000000e9, 0xbcccbbce,
	0x99bceeed, 0xcbabbbaa, 0xccdcaacc, 0xbb9cccec,
	0xcaabeeec, 0xedbacdee, 0xbbaaa99c, 0xcccba99a,
	0xbaabccba, 0xeec7addc, 0xbdecb99c, 0xaa347579,
	0xeeec87b8, 0x79acce0e, 0xe00000e9, 0xdccccbbc,
	0x999bceee, 0xccba9999, 0xcccdcaac, 0xcbb9ccdd,
	0xecbabeee, 0xbeecaacd, 0x9bbaaa99, 0xacdccba9,
	0xbaabccba, 0xeec79ddc, 0x9cedb99c, 0x8c526759,
	0xede978cb, 0x89bcc0ed, 0xce0000da, 0xedbbccbb,
	0x8999bcee, 0xcccb9878, 0xdbccdc9a, 0xecba9dcd,
	0xcdccbbee, 0x9beeca9a, 0x99abaaa9, 0xaacdccba,
	0xbbabcdcb, 0xe0d89ddc, 0x9addc99c, 0x7b924757,
	0xeda79ced, 0x99bcd0ee, 0xbce00eca, 0xeedbbdcb,
	0x77899bde, 0xacccb977, 0xecbbcdc9, 0xedcbaadc,
	0x9acccbbd, 0x99ceedc9, 0xba9abaaa, 0xa9bddccb,
	0xcb9bcdcc, 0xe0d88cdd, 0x99ceca8b, 0x98b42665,
	0xdb79dddd, 0x99bcd0ee, 0xbbce0eca, 0xeeecbcdc,
	0x777799bd, 0x9acccb98, 0xcecabcdc, 0xdedcbaac,
	0xa99bcccb, 0xa99ceeec, 0xcba9abba, 0xba9cddcc,
	0xcb9bcedc, 0xe0e97cdd, 0x79beca8a, 0xc7a72475,
	0xc79cc99b, 0x99bcd0ed, 0xcbbce0ec, 0xeeeecbcd,
	0x9877789c, 0xca9bccc9, 0xdcecaabc, 0xbceecbaa,
	0xeca99bcc, 0xaa99ceee, 0xccbbaabb, 0xcb99cddc,
	0xcb9aceed, 0xe0e97cdd, 0x69adca8a, 0x987a4156,
	0x98ca7777, 0xb9acdeed, 0xdcabce0e, 0xeeeedabd,
	0xaaa9878a, 0xccb9bccb, 0xbcceda9a, 0xcbcdecba,
	0xeeca99ac, 0xbaa99bce, 0xccccb99b, 0xcca9bddc,
	0xcb9aceee, 0xd0ea7cdd, 0x579cdb89, 0x78697126,
	0x7cb88778, 0xebacdeec, 0xeebabc00, 0xeeeeebac,
	0xbcccb978, 0x9bcb9cdc, 0xacccedb9, 0xccccdecb,
	0xeeedb999, 0xbbaa99ac, 0xcccbcb9a, 0xdcb9acdc,
	0xcbaaceee, 0xd0ea7bdd, 0x679cdb89, 0x7976a414,
	0x9c872114, 0x0dbcceea, 0xeedbcac0, 0xee0eecac,
	0xcccddcb9, 0xa9acb9cd, 0xbabccdec, 0x9bcccdec,
	0xbdeeeb99, 0xacbaaa99, 0xccccccb9, 0xdcc99cdd,
	0xcaaaceee, 0xc0eb7add, 0x568bdb99, 0x27947921,
	0xa9722221, 0xeedcced9, 0xeeeedcbc, 0xeee0ecac,
	0xcccddedc, 0xeb9acb9c, 0xcbaaccce, 0x98accbce,
	0x9ace0daa, 0x9acbaaaa, 0xddccbcca, 0xedca9cdd,
	0xcaaacdee, 0xceec79dd, 0x357adc99, 0x12543871,
	0xa8322122, 0xcdecced9, 0xde00eedc, 0xeee0edbc,
	0xcccccdee, 0xdec99cba, 0xecbaaccc, 0xa98bdcbd,
	0xaaaceeca, 0xa9bcbbba, 0xdcccbccc, 0xedcbaccd,
	0xcaa9cdee, 0xceec89dd, 0x1579cc98, 0x21111494,
	0x95222321, 0xccdecdd9, 0xdeeeeccc, 0xeee0edbc,
	0xbcccccdd, 0xccec9aca, 0xcecbbabc, 0xa977bdcc,
	0xbbbbceeb, 0xbaabccbb, 0xdccccccc, 0xeeccbccd,
	0xdaa9cdee, 0xbeec98cd, 0x2369cc98, 0x22111268,
	0x94227985, 0xdccdddd9, 0xddddccdd, 0xdee0ecbc,
	0x9ccccccc, 0xdbdec9bc, 0xccedcbab, 0xa8778cdd,
	0xbbbbbcec, 0xccbabccc, 0xcccccccc, 0xeedccccd,
	0xdaa9cddd, 0xbe0d98cd, 0x5258cc98, 0x97422337,
	0x7326cdcc, 0xeedcddca, 0xeeccdeee, 0xdee0ecac,
	0xabcccccc, 0xadbdeb9c, 0xdccddcba, 0xc8789ace,
	0xccbcbbcd, 0xbbbbaabc, 0xcccccbbb, 0xeedccccc,
	0xdaa9cdcc, 0xae0da8cd, 0x8347bca9, 0xbbb96324,
	0x624bccbb, 0xceedddc9, 0xecceedcc, 0xcee0ecad,
	0xabcccccc, 0xacccec9b, 0xedcbcecb, 0xdb89accd,
	0xccccccbc, 0x99aaa9ac, 0xccba9999, 0xcedccccc,
	0xdba9cddc, 0x9d0eb8bd, 0x7547aca9, 0x6779a952,
	0x626cb976, 0x9bcedeca, 0xcbeeca99, 0xcee0ecad,
	0xaccccccc, 0xbacced9b, 0xeedcbcdc, 0xbca9bcdd,
	0xcccccccb, 0x7899999b, 0xcb999888, 0xcdddcccc,
	0xdba9bddd, 0x9d0ec8ad, 0x68469cb9, 0x555579a6,
	0x538c9545, 0x99adedda, 0xbcec9889, 0xcee0ebac,
	0xabcccbbc, 0xbbccdeaa, 0xeeedcbdd, 0xcccaadee,
	0x9abccccc, 0x88777889, 0xb9889999, 0xccddccdc,
	0xdba9bddd, 0x9ce0c9ad, 0x99459cc9, 0x7777569b,
	0x64994467, 0x779bddda, 0xbec98877, 0xcee0ebaa,
	0xaaccccbc, 0xcbbcceba, 0xdddddcbd, 0xccccbbee,
	0x8889abbc, 0x99878888, 0xa88999a9, 0xcbcddcdc,
	0xdca9bcde, 0x9be0e99c, 0xcc648cc9, 0xaa998659,
	0x64a64789, 0x5679cdca, 0xcea88655, 0xcde0eb99,
	0xb9bcccbb, 0xcbbccec9, 0xccdccecc, 0xbccccbce,
	0x9999889a, 0xba999999, 0x9789aabb, 0xebbcdddc,
	0xdca9acee, 0x9ae0ea9c, 0xad847cca, 0xa9999866,
	0x749568ab, 0x5568accb, 0xec987544, 0xdde0eb99,
	0xabacccbb, 0xcbacbee9, 0xccdccddb, 0x9bcccbbc,
	0xaaa99877, 0xccbbbbaa, 0x9789bbcc, 0xecbcdddc,
	0xdca9acee, 0x99d00c9c, 0x7db47bda, 0x99988986,
	0x759579ca, 0x55679cdc, 0xeb875445, 0xdde0eb9b,
	0x9babcbbb, 0xccbcccec, 0xccdddddc, 0x789bccbc,
	0xbbbaaa98, 0xcccccccb, 0x979abccc, 0xeebcdddc,
	0xdcb99cee, 0xa9ce0c9c, 0x6bc55adb, 0x99998897,
	0x95857bb9, 0x55667acc, 0xd9865555, 0xdde0eb9c,
	0xa9babbbb, 0xccbbcbee, 0xcccdedec, 0x9878accb,
	0xbaa99aaa, 0xcccccccc, 0x979acccc, 0xeecccddc,
	0xdcb99cde, 0xa9be0e9b, 0x77c749cc, 0xaaa99878,
	0xa5757ba9, 0x766568cc, 0xc9755557, 0xede0ecad,
	0xd9aaabbb, 0xbcbacbce, 0xbccdeeed, 0xbb9889bc,
	0xcba9889a, 0xcccccccc, 0x979bcccc, 0xeeeccddc,
	0xddca9bde, 0xaaad0eaa, 0x76b948cc, 0xaabaa988,
	0xb7667baa, 0x5676779c, 0xc8755576, 0xede0ecbd,
	0xe99baabb, 0xbcbbccae, 0xcbcdeeed, 0x9aa9989b,
	0xcbb99877, 0xcccccccc, 0x979bcccc, 0xeeedcddc,
	0xcdca9bde, 0xbaace0b9, 0x869b46bd, 0xabccca97,
	0xc8567baa, 0x7777778c, 0xb8655666, 0xedeeeccd,
	0xec8ababb, 0xccbbbdbc, 0xbcbcdddd, 0x77999879,
	0xbbba9987, 0xcccccccc, 0x979bcccc, 0xeeeedcdc,
	0xcdcb9ade, 0xcaabe0c9, 0x777b759c, 0xacdedca9,
	0xca566aba, 0x8987787a, 0xa7555657, 0xedeeeccd,
	0xed99baab, 0xcbbbbcca, 0x8bbcddde, 0x87777777,
	0xbbbaa999, 0xcccccccc, 0xa78bcccc, 0xeeeeedcd,
	0xcdcb99ce, 0xcabacee9, 0x7769948c, 0xbcedeeca,
	0xcc7569ba, 0xa9997778, 0xa7556579, 0xeddeeccd,
	0xdeb79aaa, 0xcababcda, 0x69bbcddd, 0x98777656,
	0xba999999, 0xcccccccc, 0xc77bcccc, 0xeeeeedcd,
	0xadcc99cd, 0xdbaaaeea, 0x9777a56b, 0xbddbceec,
	0xac9567ba, 0x75698777, 0x97555589, 0xeede0dcd,
	0xcee98aba, 0xdababbea, 0x569bcddd, 0x99987765,
	0xa9777889, 0xbbcccccb, 0xc86acccc, 0xeeeeeecc,
	0xaddca9bd, 0xdcab9cec, 0xa7769749, 0xaccaadee,
	0x9cb6679b, 0x32458877, 0x97555697, 0xeede0ecd,
	0x9dec78bb, 0xebbaabdc, 0x457bcdee, 0x99999776,
	0x97667789, 0xbbcccbbb, 0xca69ccbb, 0xeeeeeedc,
	0x9cdcb9ad, 0xcdbbb9dd, 0xc9767947, 0xacca9cee,
	0x7ac9577b, 0x12456876, 0x96555794, 0xccceeecd,
	0xbadea79a, 0xecaaabce, 0x6459bcee, 0x99999988,
	0x97557779, 0xabbcccbb, 0xcc87ccca, 0xeeeeeeec,
	0xacdcc99c, 0x9dcab9be, 0xec876965, 0xbbca9d0e,
	0x68bb7579, 0x25776777, 0x96555972, 0xcccdeecc,
	0xebbec89b, 0xed9a9bbe, 0x9647bcee, 0x99999999,
	0xa8555778, 0xaaacccba, 0xccb79dcb, 0xdeeeeeee,
	0xbadcca8b, 0x7cdbbb9d, 0xeea77784, 0x9bcb9d00,
	0x779ca667, 0x47998777, 0x96556951, 0xedcdeedc,
	0xedbdc79b, 0xedaa9bad, 0x9955acde, 0x99a99a99,
	0xa9655577, 0xa9abcccb, 0xdcc97bdc, 0xceeeeeee,
	0xc9cdcb99, 0x5adcabbc, 0x0ec87685, 0x7accad00,
	0x7679c956, 0x59aaa987, 0x96557921, 0x00edeeec,
	0xeeccd77a, 0xeeb99abb, 0x99858cce, 0x79999aa9,
	0xa9755566, 0xca9abccc, 0xedcc98cd, 0xadeeeeee,
	0xd9cdcc98, 0x48cdbabb, 0x0eeb7777, 0x79bcad00,
	0x8777ac85, 0x7abccba9, 0xa6557712, 0xce0edeed,
	0xcecceb79, 0xeec999ca, 0xa9a66bce, 0x57999aba,
	0xba975665, 0xdcbaabbc, 0xeedcc98c, 0x9beeeeee,
	0xdbaddcc9, 0x55addabb, 0x000e9767, 0x579cbc00,
	0xa9777ac7, 0x9bcddccc, 0xa6557514, 0x9be0eeed,
	0xceeacd88, 0xeed998bb, 0xaaa859cd, 0x56899aba,
	0xcba97667, 0xcdccbbbb, 0xdeeccb99, 0x99ceeedd,
	0xcd9cdccb, 0x747cecab, 0xeeeec977, 0x668bbc00,
	0xba9778bb, 0x9cddcaaa, 0xb7557415, 0x57ad0eee,
	0xbeecadb8, 0xeee9989c, 0xaaaa68cc, 0x757999aa,
	0xcbba9767, 0x9bddcccc, 0xddddccb9, 0xa99ceeed,
	0xbdbaddcc, 0x7559deba, 0xeeeeec76, 0x9579cc0e,
	0x99a9879c, 0xacda8778, 0xc7557416, 0x3459e00e,
	0xbeeebcc9, 0xeeeb889c, 0xbaab87bc, 0x7767999a,
	0xcbaaa987, 0xa9accccc, 0xdddddccb, 0xca99cdee,
	0xacd9bddc, 0x7747cedb, 0xeeeeee96, 0xc869bcee,
	0x99999879, 0xacb87899, 0xc7557317, 0x33359e0e,
	0xbceeecc9, 0xdeec898c, 0xababa79c, 0x8877799a,
	0xaaaa9998, 0xcb99abba, 0xeeeddccc, 0xccba9acd,
	0xabdc9bcd, 0x67548cec, 0xddeeeec7, 0xab77bcee,
	0xccb99997, 0xbc989acc, 0xd8557217, 0x554449ee,
	0xcbeeecc9, 0xceed888b, 0xaabbb98b, 0x89887799,
	0xabbba999, 0xccbaaaaa, 0xceeeddcc, 0xccccb99b,
	0xcacdcaac, 0x667459de, 0xbbccdee9, 0x8b969ced,
	0xbbbba998, 0xcb89accb, 0xe9557228, 0x765545ae,
	0xcaceedc9, 0xceed9799, 0xaaaaba8a, 0x98997799,
	0xbcccbba9, 0xcccccbbb, 0xacdeeedc, 0xabcccca9,
	0xebacdcba, 0x766746be, 0x9999bcec, 0x89a79cec,
	0xccccba99, 0xca89ccac, 0xea656129, 0xa976546c,
	0xccbdeec9, 0xcdedb799, 0xaaabbb99, 0xa9999779,
	0xcccccccb, 0xcccccccc, 0xbabcdeed, 0xcbbbbccb,
	0xedcbbccc, 0xa757647b, 0x89999ade, 0x98b78ce9,
	0xeeeedcb9, 0xc99ccbce, 0xec756129, 0xccb97547,
	0xbcaceeb9, 0xbceec789, 0xaaaabca9, 0xcca99879,
	0xccccdddd, 0xdddccccc, 0xcbaabcee, 0xcccccbbb,
	0xbdeccccc, 0xd9757547, 0x999999be, 0x98997cd9,
	0xeeeeeecb, 0xb9acbdee, 0xad75514a, 0xcccca855,
	0xaccceeb9, 0xaceed97a, 0x99aabbc9, 0xddcb9978,
	0xdddddeee, 0xeddddddd, 0xbbcbaacd, 0xcccccccc,
	0x59bccccc, 0xec865754, 0x99bcc99c, 0xa9997cc9,
	0xeddddeec, 0xa9cbceee, 0x7d95514b, 0xabbccb85,
	0xaccbeeb9, 0x9ccedc79, 0x999abbcb, 0xeedcc997,
	0xddedeeee, 0xcddddddd, 0xccbbba9b, 0x99999acc,
	0x55678889, 0xceb76566, 0x9adedc9a, 0xc9997cc9,
	0xccccccde, 0x99cbeeed, 0x5aa5515b, 0x99999aa7,
	0xbccbeeb9, 0xabcdec88, 0x9999abbc, 0xedcccc98,
	0xccddeeee, 0xaccddccc, 0xbccbbba9, 0x78889999,
	0x66566677, 0xbdda7655, 0x9cdcdca9, 0xda998ac9,
	0xbaabbccd, 0x9bbceecb, 0x67b6516b, 0x77777799,
	0xaccbdeb9, 0xbacceda7, 0x89999abc, 0xdcccccb9,
	0xbbbcccdd, 0x99abccba, 0x99accbbb, 0x99999999,
	0x66677889, 0xacddb976, 0x9ca9bdca, 0xdc9999c9,
	0x98899abc, 0x9bbeeca9, 0x8697517b, 0x99998778,
	0x9cdbdec9, 0xcbbccdc7, 0x89989abb, 0xcbbbbcca,
	0xbbbbbbbc, 0xba999999, 0xa999bccb, 0xbbbbbbba,
	0x999999aa, 0xaacddcba, 0x9b999cca, 0xcdb998cb,
	0x777779ab, 0x9bceeb97, 0x8779517a, 0xa9999877,
	0x9cdccec8, 0xccbbcdc8, 0x98987abb, 0xbbbccccc,
	0xccaaccbb, 0xbbb9999a, 0xbba99acc, 0xcccccccc,
	0xbbaabccc, 0xaaaccccc, 0x9a9babca, 0xcdc999bc,
	0x8888778a, 0xabdec977, 0x7779517a, 0x99877887,
	0x7cdcced8, 0xbccbccd9, 0xa89979ab, 0xcccccccc,
	0xcca9cccc, 0xcbbbaaab, 0xcccb999c, 0xcccccccc,
	0xbbcccccc, 0xababbbbb, 0xa9bedbcb, 0xaccb999c,
	0x99999878, 0xaceeb878, 0x7778627a, 0x65444578,
};
