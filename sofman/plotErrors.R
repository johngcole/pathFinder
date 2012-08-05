# R program to do a quick plot of error on the trip

tst <- read.table("errors.txt",head=F)

barxerr <- mean(tst$V1)
tst$vval <- (barxerr - tst$V1)^2
tst$e2 <- tst$V1^2
stderr <- sqrt(sum(tst$vval)) / sqrt(length(tst[,1]))
erridx <- sqrt(sum(tst$e2)) / sqrt(length(tst[,1]))
jpeg("errPlot.jpg",width=1240,height=800)
plot(1:length(tst[,1]),tst$V1,type="l",ylim=range(tst$V1),xlab="Simulation Step",ylab="Error Value")
segments(1,0,length(tst$V1),0,lty=3)
text(50,0.8*range(tst$V1)[2],paste("mean err ",round(barxerr,3)," stderr of mn ",round(stderr,3),
                                   "\nerror index ",round(erridx,3),sep=""),pos=4)
title("Path Error during Simulation")
dev.off()
