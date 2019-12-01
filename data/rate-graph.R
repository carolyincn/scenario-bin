# Copyright (c) 2012,2015  Alexander Afanasyev <alexander.afanasyev@ucla.edu>

# install.packages('ggplot2')
library(ggplot2)
# install.packages('scales')
library(scales)

# install.packages('doBy')
library(doBy)

#########################
# Rate trace processing #
#########################
data = read.table("rate-trace.txt", header=T)
data$Node = factor(data$Node)
data$FaceId <- factor(data$FaceId)
data$PacketRaw <- data$PacketRaw
data$Type = factor(data$Type)

# exlude irrelevant types
data = subset(data, Type %in% c("OutInterests", "InData", "InNacks", "InInterests"))

# combine stats from all faces
data.combined = summaryBy(. ~ Time + Node + Type, data=data, FUN=sum)

root1 = subset(data.combined, Type=="OutInterests")
root2 = subset(data.combined, Type=="InData")

df = data.frame(Time = root1$Time, Node = root1$Node, Rate = root2$PacketRaw.sum / root1$PacketRaw.sum)
# df.test = subset(df, Node %in% c("H1", "H3", "H5", "H7", "H9", "H11", "H13", "H15", "H16"))
df.test = subset(df, grepl("^good-H", Node))
df.avg = summaryBy(. ~ Time, data=df.test, FUN=mean)
write.table(df.avg$Rate.mean, "rate.txt", row.names = FALSE, col.names = FALSE)
# graph rates on all nodes in PacketRaw
fa = levels(df$Node)

pdf("rate.pdf")
g.root <- ggplot(df.avg) +
  geom_point(aes (x=Time, y=Rate.mean), size=2) +
  geom_line(aes (x=Time, y=Rate.mean), size=0.5) +
  ylab("SR")

print(g.root)

### SR of all nodes ###
for(i in 1:ceiling(length(fa)/16)){
  sub_class=fa[c(1:16)+(i-1)*16]
  g.root <- ggplot(subset(data.combined, Node %in% sub_class)) +
    geom_point(aes (x=Time, y=PacketRaw.sum, color=Type), size=1) +
    geom_line(aes (x=Time, y=PacketRaw.sum, , color=Type), size=0.5) +
    ylab("Packets") +
    facet_wrap(~ Node)
  print(g.root)
}

### data of all nodes ###
# for(i in 1:ceiling(length(fa)/16)){
#   sub_class=fa[c(1:16)+(i-1)*16]
#   g.all <- ggplot(subset(data.combined, Node %in% sub_class)) +
#     geom_point(aes (x=Time, y=PacketRaw.sum, color=Type), size=1) +
#     geom_line(aes (x=Time, y=PacketRaw.sum, color=Type), size=0.5) +
#     ylab("Packets") +
#     facet_wrap(~ Node)
#   print(g.all)
# }



# graph rates on the root nodes in Packets
# g.root <- ggplot(data.root) +
#   #geom_point(aes (x=Time, y=PacketRaw.sum, color=Type), size=2) +
#   geom_line(aes (x=Time, y=PacketRaw.sum, color=Type), size=0.5) +
#   ylab("Packets")

# print(g.root)

# png("src/ndnSIM/docs/source/_static/root-rates.png", width=500, height=250)
# print(g.root)
retval <- dev.off()