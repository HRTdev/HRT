use strict;

my $datafile = $ARGV[0] || "./Fort3.DAT";
my $newdata = $ARGV[1] || "./fortik.dat";
my $newnewnew = $ARGV[2] || "./FORT_NEW.DAT";
my $svg = $ARGV[3] || "./svg.html";

my (@data,@name12,@name34,@name5,@number,@cell_cnt,@inner_trs);
my (@pins_cnt,@type,@buf_num,@pin_name,@pin_eq,@pin_num);

my ($pin_name,$pin_num,$pin_eq,$name12,$name34,$name5,$number);
my ($cell_cnt,$inner_trs,$pins_cnt,$type,$buf_num);
my $k = 0;
my $cnt = 0;
my $next_name_line = 1;
my $curr_line;

open(NEW_DATA, ">", $newdata) or die $!;
open(NEWNEW, ">", $newnewnew) or die $!;
open(SVG, ">", $svg) or die $!;
open(DATA, $datafile) or die $!;
while(<DATA>){
	chomp;
	if ((m/^(F3)\s(\d+)\s(\d)/) && ($. == $next_name_line)){
		$name12=$1;
		$number=$2;
		$type=$3;
		$k = 0;
		$cnt++;
		$curr_line = $.;
	}elsif((m/^(\d{2})\s(\d{1,2})\s(\d)/) && ($. == $curr_line + 1)){
		$name34=$1;
		$cell_cnt=$2;
		$next_name_line = $. + $2*3 + 2;
		$inner_trs=$3;
	}elsif((m/^(\d)\s(\d+)\s(\d)/) && ($. == $curr_line + 2)){
		$name5=$1;
		$pins_cnt=$2;
		$buf_num=$3;
	}elsif(m/^([A-Z0-9]{1,2})\s{1,2}(\d+)\s{1,2}(\d+)/){
		$pin_name = $1;
		$pin_num = $2;
		$pin_eq = $3;
		
		$pin_name[$cnt][$k] = $pin_name;
		$pin_eq[$cnt][$k]	= $pin_eq;
		$pin_num[$cnt][$k]  = $pin_num;
		$k++;

	}elsif(m/\*\s65535\s0/){
		$pin_name[$cnt][$k] = "*";
		$pin_eq[$cnt][$k]	= 0;
		$pin_num[$cnt][$k]  = 65535;
		$k++;
	}elsif(m/\!\s0\s0/){
		$pin_name[$cnt][$k] = "!";
		$pin_eq[$cnt][$k]	= 0;
		$pin_num[$cnt][$k]  = 0;
		$k++;
	}


	@name12[$cnt]	= $name12;
	@name34[$cnt]	= $name34;
	@name5[$cnt]	= $name5;
	@number[$cnt]	= $number;
	@cell_cnt[$cnt] = $cell_cnt;
	@inner_trs[$cnt]= $inner_trs;
	@pins_cnt[$cnt]	= $pins_cnt;
	@type[$cnt]		= $type;
	@buf_num[$cnt]	= $buf_num;
}

my $text;
for (my $i = 1; $i <= $#number; $i++){
	$text  = "@name12[$i] @number[$i] @type[$i]\n";
	$text .= "@name34[$i] @cell_cnt[$i] @inner_trs[$i]\n";
	$text .= "@name5[$i] @pins_cnt[$i] @buf_num[$i]\n";
	my $j = 0;
	while ( $j < 3*@cell_cnt[$i] ){
		$text .= "$pin_name[$i][$j] $pin_num[$i][$j] $pin_eq[$i][$j]\n";
		$text .= "$pin_name[$i][$j+1] $pin_num[$i][$j+1] $pin_eq[$i][$j+1]\n";
		$text .= "$pin_name[$i][$j+2] $pin_num[$i][$j+2] $pin_eq[$i][$j+2]\n";
		$j += 3;
	}
	for (my $j = 0; $j < 3*@cell_cnt[$i]; $j++){
	#	$text .= "$pin_name[$i][$j] $pin_num[$i][$j] $pin_eq[$i][$j]\n";
	}
	print NEW_DATA $text;
}


for (my $i = 1; $i <= $#number; $i++){
	$text  = "Name: @name12[$i]@name34[$i]@name5[$i]\n";
	$text .= "Num: @number[$i]\n";
	$text .= "Type: @type[$i]\n";
	$text .= "Buff_num: @buf_num[$i]\n";
	$text .= "Cell_cnt: @cell_cnt[$i]\n";
	$text .= "Inner_tr: @inner_trs[$i]\n";
	$text .= "Pins_cnt: @pins_cnt[$i]\n";
	$text .= "Pins_inf:\n";
	my $j = 0;
	my $k = 1;
	while ( $j < 3*@cell_cnt[$i] ){
		$text .= "C${k}P1: $pin_name[$i][$j] $pin_num[$i][$j] $pin_eq[$i][$j]\n";
		$text .= "C${k}P2: $pin_name[$i][$j+1] $pin_num[$i][$j+1] $pin_eq[$i][$j+1]\n";
		$text .= "C${k}P3: $pin_name[$i][$j+2] $pin_num[$i][$j+2] $pin_eq[$i][$j+2]\n";
		$j += 3;
		$k++;
	}
	for (my $j = 0; $j < 3*@cell_cnt[$i]; $j++){
	#	$text .= "$pin_name[$i][$j] $pin_num[$i][$j] $pin_eq[$i][$j]\n";
	}
	print NEWNEW $text;
	print NEWNEW "\n";
}




my $cnt = 0;
print SVG '<?xml version="1.0" encoding="UTF-8" standalone="no"?>';
print SVG "\n";
print SVG '<!-- Created with Inkscape (http://www.inkscape.org/) -->';
print SVG "\n";
print SVG '<svg width="1800" height="36250">';
print SVG "\n";
print SVG '<defs>';

for (my $i = 1; $i <= 40; $i++){
	print SVG "\n";
	print SVG "\t<symbol id=\"cell$i\">";
	my $t = $i-1;
	while ($t >= 0){
		my $long = $t*100;
		$long += 30;
		print SVG "\n";
		print SVG "\t\t<rect x=\"$long"; print SVG '" y="30" width="20" height="30" stroke="black" stroke-width="2" fill="none"/>';
		$long += 50;
		print SVG "\n";
		print SVG "\t\t<rect x=\"$long"; print SVG '" y="30" width="20" height="30" stroke="black" stroke-width="2" fill="none"/>';
		$long -= 80;
		print SVG "\n";
		print SVG "\t\t<rect x=\"$long"; print SVG '" y="160" width="20" height="30" stroke="black" stroke-width="2" fill="none"/>';
		$long += 50;
		print SVG "\n";
		print SVG "\t\t<rect x=\"$long"; print SVG '" y="160" width="20" height="30" stroke="black" stroke-width="2" fill="none"/>';
		$long -= 50;
		print SVG "\n";
		print SVG "\t\t<rect x=\"$long"; print SVG '" y="60" width="100" height="100" stroke="black" stroke-width="2" fill="none"/>';
		$t--;
	}
	print SVG "\n";
	print SVG "\t</symbol>";
}
print SVG "\n";
print SVG '</defs>';
print SVG "\n";


my $cnt = 0;
for (my $i = 1; $i <= $#number; $i++){
my $tmp;
	if ((@cell_cnt[$i] <= 40) && (@type[$i] != 2)){
		$tmp = 250*$cnt;
		print SVG "<g id=@name12[$i]@name34[$i]@name5[$i] transform=\"translate(0 $tmp)\">";
		print SVG "\n";
		print SVG "\t<use xlink:href=\"#cell@cell_cnt[$i]\"/>";
		print SVG "\n";
		my $j = 0;
		my $k = 1;
		while ( $j < 3*@cell_cnt[$i] ){
		my $temp = ($k-1)*100+30;
			print SVG "\t<text x=\"$temp\" y=\"25\">$pin_name[$i][$j]</text>";
			print SVG "\n";
			if($pin_name[$i][$j] eq "!"){
				my $tr = ($k-1)*100;
				print SVG "\t";
				$tr += 10;
				print SVG "<line x1=\"$tr\"";
				$tr+=30;
				print SVG "y1=\"160\" x2=\"$tr\" y2=\"60\"";
				print SVG  'style="stroke:rgb(0,0,0);stroke-width:2;"stroke-dasharray="5,5"/>';
				print SVG "\n";
			}
			$temp+=50;
			print SVG "\t<text x=\"$temp\" y=\"25\">$pin_name[$i][$j+1]</text>";
			print SVG "\n";
			$temp-=80;
			print SVG "\t<text x=\"$temp\" y=\"210\">$pin_name[$i][$j]</text>";
			print SVG "\n";
			$temp+=50;
			print SVG "\t<text x=\"$temp\" y=\"210\">$pin_name[$i][$j+2]</text>";
			print SVG "\n";
			$j += 3;
			$k++;
		}
		print SVG "\t<text x=\"30\" y=\"115\">@name12[$i]@name34[$i]@name5[$i]</text>";
		print SVG "\n";
		print SVG '</g>';
		$cnt++;
		print SVG "\n";
	}
}


close(DATA);
close(NEW_DATA);
close(NEWNEW);
close(SVG);