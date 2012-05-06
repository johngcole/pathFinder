data <- read.table("c1Track.csv",head=F,sep=",")
names(data) <- c("i","px","py","pz")

#wp <- c(0.0,0.0,3.1,0.9,5.75,5.4,11.75,18.5,26.0,24.5)
#wpm <- matrix(wp,byrow=T,ncol=2)

wpm <- data.frame(matrix(scan("waypoints.dat"),ncol=2,byrow=T))
names(wpm) <- c("px","py")

obs <- data.frame(matrix(scan("obstacles.dat"),ncol=10,byrow=T))
names(obs) <- c("type","length","width","height","density","px","py","pz","axis","rotation")


jpeg("sim-model.jpg",height=600,width=600)
plot(obs$px,obs$py,cex=0.1,pch=1,xlim=c(0,max(obs$px)),ylim=c(0,max(obs$py)),type="n")
points(wpm$px,wpm$py,pch=4,col="blue",cex=0.8)
symbols(obs$px[obs$type < 3],obs$py[obs$type < 3],rectangles=matrix(c(obs$length[obs$type < 3],obs$width[obs$type < 3]),ncol=2,byrow=F),inches=F,add=T)
symbols(obs$px[obs$type > 2],obs$py[obs$type > 2],circles=obs$width[obs$type > 2],inches=F,add=T)

# first leg
segments(0,0,wpm[1,1],wpm[1,2],col="blue")

for (i in 1:(length(wpm[,1])-1)) 
  segments(wpm[i,1],wpm[i,2],wpm[i+1,1],wpm[i+1,2],col="blue")

points(data$px,data$py,pch=16,cex=0.3)

dev.off()

quit(save="no")


dtst <- c(2.35,2.85,2.65,2.85)
dtst <- c(0.473598,0.283502,4.517829,2.850000,2.350001,2.850000,2.650002,2.850000)

dtst <- c(1.605732,1.792148,2.439941,2.850002,2.350003,2.850002,2.650003,2.850002)

dtst <- c(2.683555,4.642136,4.711049,9.2126,4.8895,9.35289,2.683555,4.642136)
dtst <- c(0.473598,0.283502,2.350001,1.474281,2.350001,3.150001,2.350001,2.850000)
dtst <- c(2.67455,4.59599,4.71608,9.16322,4.8896,9.3537,2.6745,4.9899)

dtst <- c(2.35,2.85,2.65,2.85,2.5,2.5,2.5,3.5)


tst <- matrix(dtst,byrow=T,ncol=2)

plot(tst[,1],tst[,2])

for (i in c(1,3)) {
  segments(tst[i,1],tst[i,2],tst[i+1,1],tst[i+1,2])
}


# line intersect formula
x1 <- tst[1,1]
x2 <- tst[2,1]
y1 <- tst[1,2]
y2 <- tst[2,2]
A1 <- y2 - y1
B1 <- x1 - x2
C1 <- A1*x1 + B1*y1

x1 <- tst[3,1]
x2 <- tst[4,1]
y1 <- tst[3,2]
y2 <- tst[4,2]
A2 <- y2 - y1
B2 <- x1 - x2
C2 <- A2*x1 + B2*y1

det <- A1*B2 - B1 * A2

x <- (B2*C1 - B1*C2)/det
y <- (A1*C2 - A2*C1)/det

# test to see if x and y are in the segments
min(x1,x2) >= x & x <= max(x1,x2)
