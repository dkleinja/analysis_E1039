#! perl
use Cwd;

my $cwd = getcwd;
my $output_dir = "\\\"${cwd}/root_files/\\\"";

my $line = $ARGV[0];

open my $index,'dimuon_schemas.list';
my @FileNames = <$index>;

print @FileNames;
my $length = @FileNames;
if($length < ($line+1)){
    print "line # too big \n";
    exit0;
}

my $file = $FileNames[$line];
chomp $file;

my @words = split(/\./, $file);
my $begin = "$file";

print "begin is $begin \n";
print "file is $file \n";
my $f1 = "${file}";

print "root -b -q getdimuon.C\\\(\\\"$f1\\\"\\\) \n";
system("root -b -q getdimuon.C\\\(\\\"$f1\\\"\\\)");
