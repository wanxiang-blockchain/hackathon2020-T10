import Router, { useRouter } from 'next/router'
import BaseLayout from '../components/BaseLayout'
import '../styles/globals.less'

function MyApp({ Component, pageProps }) {

  const router = useRouter()
  const { pathname } = router;

  const isManagement = !(pathname === '/')


  return (
    <>
      {
        !isManagement  ? (

          <Component {...pageProps} />

        ) : (
          <BaseLayout style={{height: '100%'}}>
            <Component {...pageProps} />
          </BaseLayout>
        )
      }
    </>

  )
}

export default MyApp
