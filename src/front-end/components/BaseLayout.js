import Router, { useRouter } from 'next/router'
import Link from 'next/link'
import { Layout, Menu, Breadcrumb } from 'antd';
import { TaobaoCircleOutlined, DatabaseOutlined, UnorderedListOutlined } from '@ant-design/icons'

const { Header, Content, Footer } = Layout;

const BaseLayout = (props) => {

  return (
    <Layout className="layout" style={{height:'100%'}}>
      <Header >
        <a href='/'><div className='logo'><p> 智能考试笔</p> </div> </a>
      </Header>
      <Content style={{ padding: '0 12px', height:'100%' }}>
          {
            props.children
          }
      </Content>
      <Footer style={{ textAlign: 'center' }}>Powered By WX.BlockChain + IoT </Footer>

      <style jsx>{`
      .logo {
        display: inline-block;
        color: #fff;
        font-size: 22px;
      }
      `} </style>
    </Layout>
  )
}

export default BaseLayout

