###安装客户端
本节将介绍如何安装 Geth，即 Go 语言实现的以太坊客户端。这里以 Ubuntu 16.04 操作系统
为例，介绍从 PPA 仓库和从源码编译这两种方式来进行安装。
####从 PPA 直接安装
首先安装必要的工具包。

`$ apt-get install software-properties-common`

之后用以下命令添加以太坊的源。
```
$ apt-get update
$ add-apt-repository -y ppa:ethereum/ethereum
$ apt-get update
```
最后安装 go-ethereum。

`$ apt-get install ethereum`

安装成功后，则可以开始使用命令行客户端 Geth。可用 `geth --help` 查看各命令和选项，例
如，用以下命令可查看 Geth 版本为 1.6.1-stable。
```
$ geth version
Geth
Version: 1.8.27-stable
Git Commit: 4bcc0a37ab70cb79b16893556cffdaad6974e7d8
Architecture: amd64
Protocol Versions: [63 62]
Network Id: 1
Go Version: go1.10.4
Operating System: linux
GOPATH=
GOROOT=/usr/lib/go-1.10
```
###从源码编译
也可以选择从源码进行编译安装。

安装 Go 语言环境
安装客户端

Go 语言环境可以自行访问 golang.org 网站下载二进制压缩包安装。注意不推荐通过包管理器

安装版本，往往比较旧。

如下载 Go 1.8 版本，可以采用如下命令。

`$ curl -O https://storage.googleapis.com/golang/go1.8.linux-amd64.tar.gz`

下载完成后，解压目录，并移动到合适的位置（推荐为 /usr/local 下） 。
```
$ tar -xvf go1.8.linux-amd64.tar.gz
$ sudo mv go /usr/local
```
安装完成后记得配置 GOPATH 环境变量。
```
$ export GOPATH=YOUR_LOCAL_GO_PATH/Go
$ export PATH=$PATH:/usr/local/go/bin:$GOPATH/bin
```
此时，可以通过 go version 命令验证安装 是否成功。
```
$ go version
go version go1.8 linux/amd64
```
下载和编译 Geth

用以下命令安装 C 的编译器。

`$ apt-get install -y build-essential`

下载选定的 go-ethereum 源码版本，如最新的社区版本：

`$ git clone https://github.com/ethereum/go-ethereum`

编译安装 Geth。
```
$ cd go-ethereum
$ make geth
```
安装成功后，可用 `build/bin/geth --help` 查看各命令和选项。例如，用以下命令可查看
Geth 版本为 1.6.3-unstable。

安装客户端
```
$ build/bin/geth version
Geth
Version: 1.8.27-stable
Git Commit: 4bcc0a37ab70cb79b16893556cffdaad6974e7d8
Architecture: amd64
Protocol Versions: [63 62]
Network Id: 1
Go Version: go1.10.4
Operating System: linux
GOPATH=
GOROOT=/usr/lib/go-1.10
```
###搭建测试区块链
####配置初始状态

首先配置私有区块链网络的初始状态。新建文件 /home/genesis.json ，内容如下。
```
{
"config": {
    "chainId": 22,
    "homesteadBlock": 0,
    "eip155Block": 0,
    "eip158Block": 0
},
"alloc" : {},
"coinbase" : "0x0000000000000000000000000000000000000000",
"difficulty" : "0x400",
"extraData" : "",
"gasLimit" : "0x2fefd8",
"nonce" : "0x0000000000000038",
"mixhash" : "0x0000000000000000000000000000000000000000000000000000000000000000",
"parentHash" : "0x0000000000000000000000000000000000000000000000000000000000000000",
"timestamp" : "0x00"
}
```
其中， chainId 指定了独立的区块链网络 ID，不同 ID 网络的节点无法互相连接。配置文件
还对当前挖矿难度 difficulty 、区块 Gas 消耗限制 gasLimit 等参数进行了设置。
####启动区块链
用以下命令初始化区块链，生成创世区块和初始状态。
```
$ mkdir /home/datadir

$ geth --datadir /home/datadir init /home/genesis.json
```
结果如下：
```
root@97594a094b46:/home# geth --datadir /home/datadir init /home/genesis.json
WARN [05-05|03:05:59.919] Sanitizing cache to Go's GC limits       provided=1024 updated=656
INFO [05-05|03:05:59.921] Maximum peer count                       ETH=25 LES=0 total=25
INFO [05-05|03:05:59.922] Allocated cache and file handles         database=/home/datadir/geth/chaindata cache=16 handles=16
INFO [05-05|03:05:59.939] Writing custom genesis block 
INFO [05-05|03:05:59.945] Persisted trie from memory database      nodes=0 size=0.00B time=4.584µs gcnodes=0 gcsize=0.00B gctime=0s livenodes=1 livesize=0.00B
INFO [05-05|03:05:59.946] Successfully wrote genesis state         database=chaindata                    hash=84e71d…97246e
INFO [05-05|03:05:59.946] Allocated cache and file handles         database=/home/datadir/geth/lightchaindata cache=16 handles=16
INFO [05-05|03:05:59.969] Writing custom genesis block 
INFO [05-05|03:05:59.970] Persisted trie from memory database      nodes=0 size=0.00B time=4.957µs gcnodes=0 gcsize=0.00B gctime=0s livenodes=1 livesize=0.00B
INFO [05-05|03:05:59.972] Successfully wrote genesis state         database=lightchaindata                    hash=84e71d…97246e

```
其中， --datadir 指定区块链数据的存储位置，可自行选择一个目录地址。

接下来用以下命令启动节点，并进入 Geth 命令行界面。
```
$ geth --identity "TestNode" --rpc --rpcport "8545" --datadir /home/datadir --port
"30303" --nodiscover console
```
各选项的含义如下:

--identity ：指定节点 ID；

--rpc ： 表示开启 HTTP-RPC 服务；

--rpcport ： 指定 HTTP-RPC 服务监听端口号（默认为 8545） ；

--datadir ： 指定区块链数据的存储位置；

--port ： 指定和其他节点连接所用的端口号（默认为 30303） ；

--nodiscover ： 关闭节点发现机制，防止加入有同样初始配置的陌生节点；

```
root@97594a094b46:/home# geth --identity "TestNode" --rpc --rpcport "8545" --datadir /home/datadir --port "30303" --nodiscover console 
WARN [05-05|03:14:08.376] Sanitizing cache to Go's GC limits       provided=1024 updated=656
INFO [05-05|03:14:08.378] Maximum peer count                       ETH=25 LES=0 total=25
INFO [05-05|03:14:08.379] Starting peer-to-peer node               instance=Geth/TestNode/v1.8.27-stable-4bcc0a37/linux-amd64/go1.10.4
INFO [05-05|03:14:08.379] Allocated cache and file handles         database=/home/datadir/geth/chaindata cache=328 handles=524288
INFO [05-05|03:14:08.413] Initialised chain configuration          config="{ChainID: 22 Homestead: 0 DAO: <nil> DAOSupport: false EIP150: <nil> EIP155: 0 EIP158: 0 Byzantium: <nil> Constantinople: <nil>  ConstantinopleFix: <nil> Engine: unknown}"
INFO [05-05|03:14:08.414] Disk storage enabled for ethash caches   dir=/home/datadir/geth/ethash count=3
INFO [05-05|03:14:08.414] Disk storage enabled for ethash DAGs     dir=/root/.ethash             count=2
INFO [05-05|03:14:08.414] Initialising Ethereum protocol           versions="[63 62]" network=1
INFO [05-05|03:14:08.578] Loaded most recent local header          number=0 hash=84e71d…97246e td=1024 age=50y3w3h
INFO [05-05|03:14:08.578] Loaded most recent local full block      number=0 hash=84e71d…97246e td=1024 age=50y3w3h
INFO [05-05|03:14:08.578] Loaded most recent local fast block      number=0 hash=84e71d…97246e td=1024 age=50y3w3h
INFO [05-05|03:14:08.578] Regenerated local transaction journal    transactions=0 accounts=0
INFO [05-05|03:14:08.603] New local node record                    seq=1 id=9cd666528486ff08 ip=127.0.0.1 udp=0 tcp=30303
INFO [05-05|03:14:08.603] Started P2P networking                   self="enode://057be18c3cf0d76065192e7891631adb19fc6483f8264f5ea134d73ff692c2978904328edc37b7f53974341702ebf6e3324ac685f706b7b149966433d24ebfcf@127.0.0.1:30303?discport=0"
INFO [05-05|03:14:08.605] IPC endpoint opened                      url=/home/datadir/geth.ipc
INFO [05-05|03:14:08.611] HTTP endpoint opened                     url=http://127.0.0.1:8545  cors= vhosts=localhost
Welcome to the Geth JavaScript console!

instance: Geth/TestNode/v1.8.27-stable-4bcc0a37/linux-amd64/go1.10.4
 modules: admin:1.0 debug:1.0 eth:1.0 ethash:1.0 miner:1.0 net:1.0 personal:1.0 rpc:1.0 txpool:1.0 web3:1.0

> 
```
####创建账号
用上述 geth console 命令进入的命令行界面采用 JavaScript 语法。可以用以下命令新建一
个账号。
```
> personal.newAccount()
Passphrase: 
Repeat passphrase: 
"0xc983829964d01fa90a312cef75988c956775ec67"

```
输入两遍密码后，会显示生成的账号，如 "0xc983829964d01fa90a312cef75988c956775ec67" 。

可以用以下命令查看该账号余额

```
> myaddress="0xc983829964d01fa90a312cef75988c956775ec67"
"0xc983829964d01fa90a312cef75988c956775ec67"
> eth.getBalance(myaddress)
0
```

看到该账号当前余额为 0。可用 `miner.start()` 命令进行挖矿，由于初始难度设置的较小，
所以很容易就可挖出一些余额。 `miner.stop()` 命令可以停止挖矿。
####创建和编译智能合约
以 Solidity 编写的智能合约为例。为了将合约代码编译为 EVM 二进制，需要安装 Solidity 编
译器 solc。

`$ apt-get install solc`

新建一个 Solidity 智能合约文件，命名为 testContract.sol ，内容如下。该合约包含一个方
法 multiply，作用是将输入的整数乘以 7 后输出。
```
```
pragma solidity ^0.5.8;
contract testContract {
    function multiply(uint a) public returns(uint d) {
    d = a * 7;
    }
}
```
用 solc 获得合约编译后的 EVM 二进制码。
```
$ solc --bin testContract.sol
======= testContract.sol:testContract =======
Binary: 
6080604052348015600f57600080fd5b5060a58061001e6000396000f3fe6080604052348015600f57600080fd5b506004361060285760003560e01c8063c6888fa114602d575b600080fd5b605660048036036020811015604157600080fd5b8101908080359060200190929190505050606c565b6040518082815260200191505060405180910390f35b600060078202905091905056fea165627a7a723058203ec941fa177944df347099bee893d3742b203efaf888831876e2e95a580406df0029

```
再用 solc 获得合约的 JSON ABI（Application Binary Interface） ，其中指定了合约接口，包
括可调用的合约方法、变量、事件等。
```
$ solc --abi testContract.sol
======= testContract.sol:testContract =======
Contract JSON ABI 
[{"constant":false,"inputs":[{"name":"a","type":"uint256"}],"name":"multiply","outputs":[{"name":"d","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"function"}]

```
下面回到 Geth 的 JavaScript 环境命令行界面，用变量记录上述两个值。注意在 code 前加上
0x 前缀。
```
> code = "0x6080604052348015600f57600080fd5b5060a58061001e6000396000f3fe6080604052348015600f57600080fd5b506004361060285760003560e01c8063c6888fa114602d575b600080fd5b605660048036036020811015604157600080fd5b8101908080359060200190929190505050606c565b6040518082815260200191505060405180910390f35b600060078202905091905056fea165627a7a723058203ec941fa177944df347099bee893d3742b203efaf888831876e2e95a580406df0029"
> abi = [{"constant":false,"inputs":[{"name":"a","type":"uint256"}],"name":"multiply","outputs":[{"name":"d","type":"uint256"}],"payable":false,"stateMutability":"nonpayable","type":"function"}]
```

####部署智能合约
在 Geth 的 JavaScript 环境命令行界面，首先用以下命令解锁自己的账户，否则无法发送交
易。
```
> personal.unlockAccount(myAddress)
Unlock account 0x1b6eaa5c016af9a3d7549c8679966311183f129e
Passphrase:
true
```

接下来发送部署合约的交易。
```
> myContract = eth.contract(abi)
> contract = myContract.new({from:myAddress,data:code,gas:1000000})
```
如果此时没有在挖矿，用 txpool.status 命令可看到本地交易池中有一个待确认的交易。可
用以下命令查看当前待确认的交易

如果发生此类错误
```
Error: insufficient funds for gas * price + value
    at web3.js:3143:20
    at web3.js:6347:15
    at web3.js:5081:36
    at web3.js:3021:24
    at <anonymous>:1:12

```
通过将创世区块配置文件中的chainId改成一个比较大的值，如1000,再进行测试，问题解决。

运行时的命令行中指定networkid=chainId，即--networkid 1000

```
> eth.getBlock("pending",true).transactions
[{
blockHash: "0xbf0619ca48d9e3cc27cd0ab0b433a49a2b1bed90ab57c0357071b033aca1f2cf",
blockNumber: 17,
from: "0x1b6eaa5c016af9a3d7549c8679966311183f129e",
gas: 90000,
gasPrice: 20000000000,
hash: "0xa019c2e5367b3ad2bbfa427b046ab65c81ce2590672a512cc973b84610eee53e",
input: "0x6060604052341561000c57fe5b5b60a58061001b6000396000f30060606040526000357c
0100000000000000000000000000000000000000000000000000000000900463ffffffff168063c6888fa1
14603a575bfe5b3415604157fe5b60556004808035906020019091905050606b565b604051808281526020
0191505060405180910390f35b60006007820290505b9190505600a165627a7a72305820748467daab52f2
f1a63180df2c4926f3431a2aa82dcdfbcbde5e7d036742a94b0029",
nonce: 1,
r: "0xbcb2ba94f45dfb900a0533be3c2c603c2b358774e5fe89f3344031b202995a41",
s: "0x5f55fb1f76aa11953e12746bc2d19fbea6aeb1b9f9f1c53a2eefab7058515d99",
to: null,
transactionIndex: 0,
v: "0x4f",
value: 0
}]
```
可以用 miner.start() 命令挖矿，一段时间后，交易会被确认，即随新区块进入区块链。

####调用智能合约

用以下命令可以发送交易，其中 sendTransaction 方法的前几个参数与合约中 multiply 方法的
输入参数对应。这种方式，交易会通过挖矿记录到区块链中，如果涉及状态改变也会获得全
网共识。

`> contract.multiply.sendTransaction(10, {from:myAddress})`

如果只是想本地运行该方法查看返回结果，可采用如下方式获取结果。
```
> contract.multiply.call(10)
70
```