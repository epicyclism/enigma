﻿// Enigma.cpp : Defines the entry point for the application.
//

#include <array>
#include <string_view>
#include "Enigma.h"

using frequency_table = std::array<int, alpha_max + 1>;

frequency_table get_frequency(const char * m)
{
	frequency_table tab;
	std::string_view msg(m);
	tab.fill(0);

	for (auto ch : msg)
	{
		if (valid_from_char(ch))
			++tab[from_printable(ch).Val()];
		else
			++tab[alpha_max];
	}
	return tab;
}
void print_tab(frequency_table const& tab)
{
	char row = 'A';
	for (auto i : tab)
	{
		std::cout << row << " " << i << "\n";
		++row;
	}
}

int main()
{
	auto tab = get_frequency("EDPUD NRGYS ZRCXN UYTPO MRMBO FKTBZ REZKM LXLVE FGUEY SIOZV EQMIK UBPMM YLKLT TDEIS MDICA GYKUA CTCDO MOHWX MUUIA UBSTS LRNBZ SZWNR FXWFY SSXJZ VIJHI DISHP RKLKA YUPAD TXQSP INQMA TLPIF SVKDA SCTAC DPBOP VHJK");
	print_tab(tab);
	std::cout << "\n";
	tab = get_frequency("AUFKL XABTE ILUNG XVONX KURTI NOWAX KURTI NOWAX NORDW ESTLX SEBEZ XSEBE ZXUAF FLIEG ERSTR ASZER IQTUN GXDUB ROWKI XDUBR OWKIX OPOTS CHKAX OPOTS CHKAX UMXEI NSAQT DREIN ULLXU HRANG ETRET ENXAN GRIFF XINFX RGTX");
	print_tab(tab);
	std::cout << "\n";

	tab = get_frequency("SFBWD NJUSE GQOBH KRTAR EEZMW KPPRB XOHDR OEQGB BGTQV PGVKB VVGBI MHUSZ YDAJQ IROAX SSSNR EHYGG RPISE ZBOVM QIEMM ZCYSG QDGRE RVBIL EKXYQ IRGIR QNRDN VRXCY YTNJR");
	print_tab(tab);
	std::cout << "\n";
	tab = get_frequency("DREIG EHTLA NGSAM ABERS IQERV ORWAE RTSXE INSSI EBENN ULLSE QSXUH RXROE MXEIN SXINF RGTXD REIXA UFFLI EGERS TRASZ EMITA NFANG XEINS SEQSX KMXKM XOSTW XKAME NECXK");
	print_tab(tab);
	std::cout << "\n";

	return 0;
}
