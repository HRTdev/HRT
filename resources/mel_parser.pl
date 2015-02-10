use strict;


my $in_file = $ARGV[0] || "./HV154k2.mel";
my $out_file = substr($in_file, 0, -4);
$out_file .= "_new.mel";

my @macro_names;
$out_file .= "_new";

parse($in_file,$out_file);
#print "@macro_names";

for (my $i = 0; $i <= $#macro_names; $i++){
	print "look ---> @macro_names[$i]\n";
	parse("@macro_names[$i].mel","@macro_names[$i].mel_new");
}
		
		
sub parse{
#print @_;
	my $memory_string = "";
	my ($datafile, $newdata) = @_;
	#my $datafile = $ARGV[0] || "./HV154k2.mel";
	#my $newdata = "$datafile._new";

	my (@in, @out, @inout, @num, @type, @pins_in, @pins_out, @pins_inout, @comments);
	my ($in, $out, $inout, $num, $type, $pins_in, $pins_out, $pins_inout, $comments);
	my ($fbib, $mbib, $cnt);

	$cnt = 0;
	open(DATA, $datafile) or die $!;
	open(NEW_DATA, ">", $newdata) or die $!;
	while(<DATA>){
		#chomp;
		#print "the string: $_ \n";
		#([a-z0-9])\s{0,1}\r\n
		if(m/^;$/){ next; }	#exception for one shitty senseless string case
			if (m/^\"(.*)\"/){
				@comments[$.-1] = $1;
			}elsif(m/FBIB\:\s(.*?)\;\sMBIB\:\s(.*?)\;/){
				$fbib = $1;
				$mbib = $2;
			}elsif(m/^IN\:\s(.*)/){
				$pins_in = $1;
			}elsif(m/^OUT\:\s(.*)/){
				$pins_out = $1;
			}elsif(m/^INOUT\:\s(.*)/){
				$pins_inout = $1;
			}else{
				$memory_string .= $_;
				if($memory_string =~ m/DD.*;\n/s){

				#print "heya: $memory_string";
			
				if($memory_string =~ m/^DD(\d{1,5})\*F(\d{4})\s((([a-zA-Z0-9'_#]*\s)*[,;])+)/){
					$num = $1;
					$type = $2;
					($in, $out, $inout) = split(/,/, $3, 3);
					$in =~ s/(^\s+)|(\s+;$)//g;
					$out =~ s/(^\s+)|(\s+;$)//g;
					$inout =~ s/(^\s+)|(\s+;$)//g;
					push @in, [ split(/\s/,$in) ];
					push @out, [ split(/\s/,$out) ];
					push @inout, [ split(/\s/,$inout) ];
					@num[$cnt] = $num;
					@type[$cnt] = $type;
					$cnt++;
					}elsif($memory_string =~ m/^DD(\d+)\*{3}									#Начало строки типа: DD123***
											([a-z0-9_#']+)\s								#Имя макроэлемента + пробел
											(
											(([a-z0-9_#']+\s{0,1}(\r\n){0,1}\s{0,1})+)		#Одно или больше имя входа элемента с пробелами
											(	
												\,\s										#Опциональный разделитель выходов от двунапарвленных.
												([a-z0-9_#']+\s{0,1}(\r\n){0,1}\s{0,1})+	#Одно или больше имя двунправленных
											){1,2}
											\;)
											\n											#Обязательное окончание строки
											/xi 												#x - позволяет писать комменты, i - игнорирование кейза
										){
											print "$1 - $2\n";
											@macro_names[$#macro_names+1] = $2;
											$num = $1;
											$type = $2;
											($in, $out, $inout) = split(/,/, $3, 3);
											$in =~ s/(^\s+)|(\s+;$)|(\n)//g;
											$out =~ s/(^\s+)|(\s+;$)|(\n)//g;
											$inout =~ s/(^\s+)|(\s+;$)|(\n)//g;
											push @in, [ split(/\s/,$in) ];
											push @out, [ split(/\s/,$out) ];
											push @inout, [ split(/\s/,$inout) ];
											@num[$cnt] = $num;
											@type[$cnt] = $type;
											$cnt++;
											
										}
										$memory_string = "";
				}
			}
	}

	####This part creates mel file identical to one readed
	my $text = "";
	for (my $i = 0; $i <= $#comments; $i++){
		$text .= "\"@comments[$i]\"\n";
	}
	$text .= "FBIB: $fbib; MBIB: $mbib;\n";
	$text .= "IN: $pins_in\n";
	$text .= "OUT: $pins_out\n";
	$text .= "INOUT: $pins_inout\n";
	$text .= ";\n";

	for (my $j = 0; $j < $cnt; $j++){
		$text .= "DD@num[$j]*F@type[$j] ";
		for (my $i = 0; $i <= $#{$in[$j]}; $i++){
			$text .= "$in[$j][$i]";
			$text .= " ";
		}
		
		for (my $i = 0; $i <= $#{$out[$j]}; $i++){
			$text .= ", " if $i == 0;
			$text .= "$out[$j][$i]";
			$text .= " ";
		}
		for (my $i = 0; $i <= $#{$inout[$j]}; $i++){
			$text .= ", " if $i == 0;
			$text .= "$inout[$j][$i]";
			$text .= " ";
		}
		$text .= ";\n";
	}

	###This part creates new mel, which can be easely parsed by Qt
	$text = "";
	for (my $j = 0; $j < $cnt; $j++){
		$text .= "DD@num[$j]*F@type[$j]\n";
		for (my $i = 0; $i <= $#{$in[$j]}; $i++){
			$text .= " IN$i: $in[$j][$i]\n";
		}
		
		for (my $i = 0; $i <= $#{$out[$j]}; $i++){
			$text .= " OUT$i: $out[$j][$i]\n";
		}
		for (my $i = 0; $i <= $#{$inout[$j]}; $i++){
			$text .= " INOUT$i: $inout[$j][$i]\n";
		}
	}
	print NEW_DATA $text;

	close(DATA);
	close(NEW_DATA);
}