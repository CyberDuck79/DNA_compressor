# DNA_compressor

## DNA compression by bits serialization
Replace ACGT chars (one byte) by two bits, so we can put 4 symbols in one bytes and compressed DNA by factor 4.  
  
USAGE : 1) flag 2) filename  
flags : -c compression, -d decompression  
file : .dna extension for compression, .dzp for decompression  
the file can only contains newline and ACGT chars.  
  


TODO : change read_all_file by buffer