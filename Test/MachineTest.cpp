// MachineTest.cpp 
//

#include <iostream>
#include <string>
#include "const_helpers.h"
#include "machine.h"

void Test1()
{
	machine3 m3 = MakeMachine3("B432");
	Ring(m3, "rit");
	Setting(m3, "VOR");
	Stecker(m3, "AH BO CG DP FL JQ KS MU TZ WY");

	auto ciphertext = make_alpha_array("FVKFC DWRII CYFHV SKQOW QTTH");
	auto out = m3.Transform(std::begin(ciphertext), std::end(ciphertext));
	for (auto c : out)
		std::cout << c;
	std::cout << "\n";
}

void Test2()
{
	machine3 m3 = MakeMachine3("B125");
	Ring(m3, "fvn");
	Setting(m3, "XWB");
	Stecker(m3, "PO ML IU KJ NH YT GB VF RE DC" );

	auto ciphertext = make_alpha_array("QBLTWLDAHHYEOEFPTWYBLENDPMKOXLDFAMUDWIJDXRJZ");

	auto out = m3.Transform(std::begin(ciphertext), std::end(ciphertext));
	for (auto c : out)
		std::cout << c;
	std::cout << "\n";
}

void Test3()
{
	auto ciphertext1 = make_alpha_array("EDPUD NRGYS ZRCXN UYTPO MRMBO FKTBZ REZKM LXLVE FGUEY SIOZV EQMIK UBPMM YLKLT TDEIS MDICA GYKUA CTCDO MOHWX MUUIA UBSTS LRNBZ SZWNR FXWFY SSXJZ VIJHI DISHP RKLKA YUPAD TXQSP INQMA TLPIF SVKDA SCTAC DPBOP VHJK" );

	machine3 m3 = MakeMachine3("B245");
	Ring(m3, "bul");
	Setting(m3, "WXC");
	Stecker(m3, "AV BS CG DL FU HZ IN KM OW RX");

	auto key1 = make_alpha_array("KCH");
	// decode the key...
	auto key = m3.Transform(std::begin(key1), std::end(key1));
	for (auto c : key)
		std::cout << c;
	std::cout << "\n";

	// set the key
	m3.Setting(key[0], key[1], key[2]);

	// decode the message
	auto out = m3.Transform(std::begin(ciphertext1), std::end(ciphertext1));
	for (auto c : out)
		std::cout << c;
	std::cout << "\n";
}

void Test4()
{
	auto ciphertext1 = make_alpha_array("SFBWD NJUSE GQOBH KRTAR EEZMW KPPRB XOHDR OEQGB BGTQV PGVKB VVGBI MHUSZ YDAJQ IROAX SSSNR EHYGG RPISE ZBOVM QIEMM ZCYSG QDGRE RVBIL EKXYQ IRGIR QNRDN VRXCY YTNJR" );

	machine3 m3 = MakeMachine3("B245");
	Ring(m3, "bul");
	Setting(m3, "CRS");
	Stecker(m3, "AV BS CG DL FU HZ IN KM OW RX");

	auto key1 = make_alpha_array("YPJ");
	// decode the key...
	auto key = m3.Transform(std::begin(key1), std::end(key1));
	for (auto c : key)
		std::cout << c;
	std::cout << "\n";
	
	// set the key
	m3.Setting(key[0], key[1], key[2]);

	// decode the message
	auto out = m3.Transform(std::begin(ciphertext1), std::end(ciphertext1));
	for (auto c : out)
		std::cout << c;
	std::cout << "\n";
}

int main()
{
	Test1();
	Test2();
	Test3();
	Test4();

	return 0;
}