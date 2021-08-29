all:
	go build mesher.go

optmesh:
	pv ../meshfiles/armadillo.mesh \
		| ./mesher > ../meshfiles/armadillo.optim.mesh

genmesh:
	./mesh-generator > ../meshfiles/100d-quad1.mesh
