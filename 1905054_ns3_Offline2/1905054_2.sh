#!/bin/bash



#directory
data_dir="./scratch/1905054_output2/flow1"
data_dir2="./scratch/1905054_output2/flow2"
plots_dir="./graph1"
plots_dir2="./graph2"

rm -r "$data_dir"
rm -r "$data_dir2"
rm -r "$plots_dir"
rm -r "$plots_dir2"

mkdir -p "$data_dir"
mkdir -p "$data_dir2"
mkdir -p "$plots_dir"
mkdir -p "$plots_dir2"

echo "data rate"
for dRate in 1 20 100 150 250 300; do
  ./ns3 run scratch/1905054_ns3_2.cc -- --flag=1 --bttlnkRate="$dRate" 
done
echo "packet Loss"
for pLoss in  0.002 0.0008 0.0002 0.00008; do
  ./ns3 run scratch/1905054_ns3_2.cc -- --flag=2 --packet_loss_rate="$pLoss"  
done




#plots
for file1 in "$data_dir"/*.txt; do
  filename1=$(basename "$file1")  
  filename_noext1="${filename1%.txt}"  
  xlabel1=$(echo "$filename_noext1" | cut -d '_' -f 2)  
  ylabel1=$(echo "$filename_noext1" | cut -d '_' -f 1) 

  for file2 in "$data_dir2/$filename_noext1.txt"; do
    if [ -f "$file2" ]; then
      gnuplot <<EOF
set terminal png size 800,600
set output '$plots_dir/$filename_noext1.png'

set title "Plot for $filename_noext1"
set xlabel "$xlabel1"
set ylabel "$ylabel1"

plot '$file1' using 1:2 with linespoints title 'NewReno' lc rgb 'red', \
     '$file2' using 1:2 with linespoints title 'WestWoodPlus' lc rgb 'green'
set output
quit
EOF
    fi
  done
done

echo "Hello"


rm -r "$data_dir"
rm -r "$data_dir2"
#rm -r "$plots_dir"
rm -r "$plots_dir2"

mkdir -p "$data_dir"
mkdir -p "$data_dir2"
#mkdir -p "$plots_dir"
mkdir -p "$plots_dir2"

echo "data rate"
for dRate in 1 20 100 150 250 300; do
  ./ns3 run scratch/1905054_ns3_2.cc -- --tcpVariant1=ns3::TcpAdaptiveReno  --flag=1 --bttlnkRate="$dRate" 
done
echo "packet Loss"
for pLoss in  0.002 0.0008 0.0002 0.00008; do
  ./ns3 run scratch/1905054_ns3_2.cc -- --tcpVariant1=ns3::TcpAdaptiveReno  --flag=2 --packet_loss_rate="$pLoss"  
done






#plots
for file1 in "$data_dir"/*.txt; do
  filename1=$(basename "$file1")  
  filename_noext1="${filename1%.txt}"  
  xlabel1=$(echo "$filename_noext1" | cut -d '_' -f 2)  
  ylabel1=$(echo "$filename_noext1" | cut -d '_' -f 1) 

  for file2 in "$data_dir2/$filename_noext1.txt"; do
    if [ -f "$file2" ]; then
      gnuplot <<EOF
set terminal png size 800,600
set output '$plots_dir2/$filename_noext1.png'

set title "Plot for $filename_noext1"
set xlabel "$xlabel1"
set ylabel "$ylabel1"

plot '$file1' using 1:2 with linespoints title 'AdaptiveReno' lc rgb 'red', \
     '$file2' using 1:2 with linespoints title 'NewReno' lc rgb 'green'
set output
quit
EOF
    fi
  done
done

echo "The end"
