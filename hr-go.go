package main

import (
	"fmt"
	"hash/crc32"
	"os"
	"strconv"
	"sync"
)

type chunk struct {
	bufsize int
	offset  int64
}

const DEFAULT_BLOCK_SIZE = 1024 * 1024 // 1 MiB by default
const MAX_GOROUTINES = 10

func main() {
	if len(os.Args) < 3 {
		fmt.Println("Incorrect number of parameters.")
		fmt.Println("Usage: ./hashing-result-go input output [1024]")
		return
	}

	file, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Println(err)
		return
	}

	defer file.Close()

	fileout, err := os.OpenFile(os.Args[2], os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0755)
	if err != nil {
		fmt.Println(err)
		return
	}

	defer fileout.Close()

  var BufferSize int

	if len(os.Args) > 3 {
		BufferSize, err = strconv.Atoi(os.Args[3])
		if err != nil {
			BufferSize = DEFAULT_BLOCK_SIZE
		}
	} else {
		BufferSize = DEFAULT_BLOCK_SIZE
	}

	fileinfo, err := file.Stat()
	if err != nil {
		fmt.Println(err)
		return
	}

	filesize := int(fileinfo.Size())

	concurrency := filesize / BufferSize

	chunksizes := make([]chunk, concurrency)

	for i := 0; i < concurrency; i++ {
		chunksizes[i].bufsize = BufferSize
		chunksizes[i].offset = int64(i)
	}
	if remainder := filesize % BufferSize; remainder != 0 {
		c := chunk{bufsize: remainder, offset: int64(concurrency)}
		concurrency++
		chunksizes = append(chunksizes, c)
	}

  guard := make(chan struct{}, MAX_GOROUTINES)

	var wg sync.WaitGroup
	wg.Add(concurrency)

	for i := 0; i < concurrency; i++ {
		guard <- struct{}{} // would block if guard chan filled
    go func(chunksizes []chunk, i int) {

			defer wg.Done()

			chunk := chunksizes[i]
			buffer := make([]byte, chunk.bufsize)
			_, err = file.ReadAt(buffer, int64(BufferSize)*chunk.offset)

			if err != nil {
				fmt.Println(err)
				return
			}

			// Padding leading zeros for 10 bytes
			padded := fmt.Sprintf("%010v", strconv.FormatUint(uint64(crc32.ChecksumIEEE(buffer)), 10))

			bs := []byte(padded)

			_, err = fileout.WriteAt(bs, int64(len(bs))*chunk.offset)

			if err != nil {
				fmt.Println(err)
				return
			}

      <-guard
			// fmt.Println("bytes read, string(bytestream): ", bytesread, byteswrite)
			// fmt.Println("CRC32 value: ", crc32.ChecksumIEEE(buffer));
			// fmt.Println("bytestream to string: ", chunk.offset)
		}(chunksizes, i)
	}

	wg.Wait()
}
