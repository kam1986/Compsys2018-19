(*

    32-bit adress space, mulige 2^32 adresser 
    2 way ass cache size 32 kilobyte (32*10³)
    cache blok size 16 bytes.

    ----------------------------

    m = 32 bit 
    M = 42949672964

    E = 2
    C = 32000 bytes
    B = 16 byts

    b = log_2 16 = 4 ( )

*)
// S 

let intsqrt = int << sqrt << float

let rec ToBinary value length = 
    match length with
    | 0 -> "" 
    | _ -> ToBinary (value >>> 1) (length - 1) + string "01".[value &&& 1]

let To_tio tag index offset value =
    let mutable str = "\n"
    str <- sprintf "|%s%s" (ToBinary value offset) str
    str <- sprintf "|%s%s" (ToBinary (value >>> offset) index) str 
    str <- sprintf "%s%s" (ToBinary (value >>> offset + index) tag) str
    str

let rec listToString = function
    | []    -> ""
    | x::xs -> sprintf "%s%s" (string x) (listToString xs)

let printTio tag index offset lst =
    let tio = To_tio tag index offset
    let tuple a b = (a,b)
    let table = printfn "%s"
                    ((listToString [for i in 1 .. tag -> 't']) + "|" + listToString [for i in 1 .. index -> 'i'] + "|" + listToString [for i in 1 .. offset -> 'o'])

    List.map (fun x -> tio x) lst
    |> List.fold (fun a x -> printf "%s" x) () 

let input = [0; 15728640; 0; 15728652; 12582916; 14680072; 15728644; 12582912; 8; 16; 4;
   0; 0; 15728640; 0; 15728652; 12582916; 14680072; 15728644; 12582912; 8; 16;
   4]



type test = HIT | MISS

type line =
    {
        mutable valid : bool
        mutable tag : uint64
        block : byte[] 
    }

    member L.Item
        with get(index) = 
            L.block.[index]

        and set index value = 
            L.block.[index] <- value

    member L.IsValid = L.valid

    member L.IsRightTag tag = L.tag = tag
   


type NWayCache(S, E, B) =
    
    let ran = System.Random().Next(0,10000);

    let S = S
    let E = E
    let B = B
    let C = S * E * B
    let M = pown 2 32

    let m = 32
    let s = intsqrt S   // index bit
    let b = intsqrt B   // offset bits
    let t = m - (s + b) 

    // the actual cache stucture
    // a boolean as 'valid bit'
    // a int as address checker for tag index and offset
    let cache =
        // number of sets
        [| for i in 1 .. S ->
            // number of lines pr set
            [|for j in 1 .. E ->
                // creating line with valid, tag and block
                {
                    valid = false 
                    tag = uint64 0
                    block = [|for i in 1 .. B -> byte 0|]
                 }
            |]
        |]

        
    member C.Load (adress : int) =
       ()
