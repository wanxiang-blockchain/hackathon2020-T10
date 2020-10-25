import Head from 'next/head'
import * as mqtt from '../services/mqtt'
import BackButton from '../components/BackButton'
import ExamStatistic from '../components/ExamStatistic'
import SmartPenStatistic from '../components/SmartPenStatistic'
import BlockchainStatistic from '../components/BlockchainStatistic'
import '../styles/Home.module.less'

mqtt.start()

export default function Home() {
  return (
    <div className='container'>
      <Head>
        <title>Smart Pen</title>
        <link rel="icon" href="/favicon.ico" />
      </Head>

      <BackButton />
      <main className='main'>
        <h1 className='title'>
          Welcome to <a href="/"> certification exams!</a>
        </h1>

        <p className='description' style={{display: 'none'}}>
          区块链智能考试用笔           
        </p>

        <div className='grid'>
          <a href="/live?id=9" className='card'>
            <h3>Live exams - 教室9 &rarr;</h3>
            <ExamStatistic />
          </a>

          <a href="https://testpoe.i.wxblockchain.com/#/InquiryCertificateDeposit" target='_blank' className='card'>
            <h3>区块链浏览器 &rarr;</h3>
            <BlockchainStatistic />
          </a>

          <a
            href="/verify"
            className='card'
          >
            <h3>考卷验真 &rarr;</h3>
            <SmartPenStatistic/>
          </a>

          <a
            href="/smartpen"
            className='card'
          >
            <h3>智能笔管理 &rarr;</h3>
            <ul>
              <li>人和笔身份绑定上链</li>
              <li>笔迹实时存证</li>
            </ul>
          </a>

          {/*
          <a
            href="/about"
            className='card'
          >
            <h3>关于智能笔 &rarr;</h3>
            <ul>
              <li>自动身份识别</li>
              <li>防作弊</li>
            </ul>
          </a>
          */}
        </div>
      </main>

      <footer className='footer'>
        <p>
          Powered by WX.BlockChain + IoT 
        </p>
      </footer>
    </div>
  )
}
