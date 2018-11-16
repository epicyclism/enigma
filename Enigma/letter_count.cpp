// Enigma.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <array>
#include <string_view>

#include "modalpha.h"

using frequency_table = std::array<int, alpha_max + 2>;

frequency_table get_frequency(const char * m)
{
	frequency_table tab;
	std::string_view msg(m);
	tab.fill(0);
	int cnt = 0;
	for (auto ch : msg)
	{
		if (valid_from_char(ch))
		{
			++tab[from_printable(ch).Val()];
			++cnt;
		}
		else
			++tab[alpha_max];
	}
	tab[alpha_max + 1] = cnt;
	return tab;
}
void print_tab(frequency_table const& tab)
{
	std::cout << "msg size = " << tab[alpha_max + 1] << "\n";
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
	tab = get_frequency("VONVONJSCHMIDTJFUNKVONBEFEHLSHABERDERUBOOTEXSOFORTSTANDORTMARQUANTONLUCIEVERLASSENXUUUZWOZWONEUNTREFFENAMGEGNERSTANDMARQUBRUNOEMILACHTDREIZWOEINSXBRITISCHESPANZERSCHIFFUMEINSSIEBENNULLNULLUHRDORTERWARTETXERFOLGSMELDUNGSO");
	print_tab(tab);
	std::cout << "\n";
	tab = get_frequency("VONGRUPPEWESTANARMEEGRUPPEJBJXXPLANUNGUNTERNEHMENJWACHTAMRHEINJFERTIGXSECHSSSPZARMEEUNTERJDIETRICHJMITEINSSSPZDIVJLEIBSTANDARTEADOLFHITLERJUNDEINSZWOSSPZDIVJHITLERJUGENDJXANGRIFFSSPITZERICHTUNGNORDYEINNAHMEJANTWERPENJXFUNFPZARMEEUNTERJVONMANTEUFFELJMS");
	print_tab(tab);
	std::cout << "\n";
	return 0;
}
