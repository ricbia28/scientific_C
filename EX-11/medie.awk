NF && $1 != "#"{
    n[$2]++
    if($2 > max) max = $2
    L = $1
    isPerc[$2] += $3
    percSize1[$2] += $4
    percSize2[$2] += $4 * $4
    if ($2 > $4) {
        meanSize1[$2] += ($5 - $4 * $4) / ($2 - $4)
        meanSize2[$2] += ($5 - $4 * $4) / ($2 - $4) * ($5 - $4 * $4) / ($2 - $4)
    } else {
        meanSize1[$2] += 1.0
        meanSize2[$2] += 1.0
    }
    time1[$2] += $7
    time2[$2] += $7 * $7
}
END{
    printf("#1)L  2)p  3),4)Pperc  5),6)percSize  7),8)<S>  9),10)time  11)nconf\n")
    for(i = 0; i <= max; i++) {
        if (n[i]) {
            print L, i/(L*L), isPerc[i]/n[i], sqrt(isPerc[i]-isPerc[i]*isPerc[i]/n[i])/n[i],
                percSize1[i]/n[i], sqrt(percSize2[i]-percSize1[i]*percSize1[i]/n[i])/n[i],
                meanSize1[i]/n[i], sqrt(meanSize2[i]-meanSize1[i]*meanSize1[i]/n[i])/n[i],
                time1[i]/n[i],sqrt(time2[i]-time1[i]*time1[i]/n[i])/n[i], n[i]
        }
    } 
}
