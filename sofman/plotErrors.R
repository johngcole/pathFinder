# R program to do a quick plot of error on the trip

tst <- read.table("errors.txt",head=F)

jpeg("errPlot.jpg",width=1240,height=800)
plot(1:length(tst[,1]),tst$V1,type="l",ylim=range(tst$V1),xlab="Simulation Step",ylab="Error Value")
segments(1,0,length(tst$V1),0,lty=3)
title("Path Error during Simulation")
dev.off()
