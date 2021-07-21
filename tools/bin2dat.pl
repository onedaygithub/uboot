#!/usr/bin/perl

use strict;

my $srcFile = $ARGV[0];
my $desFile = $ARGV[1]; 

open (BINFILE, "<$srcFile")||die "$!\nCannot open the bin file $srcFile\n";
binmode (BINFILE);

open (TXTTFILE, ">$desFile") ||die "$!\nCannot write to TXT file $desFile\n";

my $binData;
my @arrary;
my $i = 0;
my $count = 0;

while( read(BINFILE, $binData, 1) )
{
   my $dat = ord($binData);
   push @arrary,$dat;
}
my $size = @arrary;
if($size%4==0){}
elsif($size%4==1){push @arrary,0;push @arrary,0;push @arrary,0;}
elsif($size%4==2){push @arrary,0;push @arrary,0;}
elsif($size%4==3){push @arrary,0;}


for($i=0;$i<@arrary;$i=$i+8){
    my $byte0;
    my $byte1;
    my $byte2;
    my $byte3;
    my $byte4;
    my $byte5;
    my $byte6;
    my $byte7;
 $byte0 =  unpack("H8",pack("C",$arrary[$i+0]));
 $byte1 =  unpack("H8",pack("C",$arrary[$i+1]));
 $byte2 =  unpack("H8",pack("C",$arrary[$i+2]));
 $byte3 =  unpack("H8",pack("C",$arrary[$i+3])); 

 $byte4 =  unpack("H8",pack("C",$arrary[$i+4]));
 $byte5 =  unpack("H8",pack("C",$arrary[$i+5]));
 $byte6 =  unpack("H8",pack("C",$arrary[$i+6]));
 $byte7 =  unpack("H8",pack("C",$arrary[$i+7]));

    print TXTTFILE "\@";
    printf TXTTFILE ("%x",0x10 + $count);
    print TXTTFILE " "; 
    print TXTTFILE $byte7;
    print TXTTFILE $byte6;
    print TXTTFILE $byte5;
    print TXTTFILE $byte4;
    print TXTTFILE $byte3;
    print TXTTFILE $byte2;
    print TXTTFILE $byte1;
    print TXTTFILE $byte0;
    print TXTTFILE "\n";
    $count++;
}
   

close BINFILE;
close TXTTFILE;

